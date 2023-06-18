#include "../include/scene.h"

#include <math.h>

#include <stdexcept>

__USING_API

Scene::Scene() {
    this->_width = 30;
    this->_height = 30;
    this->_player = nullptr;
    this->_enemies = new std::vector<Enemy *>();
    this->_bullets = new std::vector<Bullet *>();
    this->_scene_sem = new Semaphore(1);

    this->_player_texture = new sf::Texture();
    this->_enemy_texture = new sf::Texture();
    this->_bullet_texture = new sf::Texture();

    if (!this->_player_texture->loadFromFile("assets/sprites/player.png")) {
        throw std::runtime_error("Could not load player texture");
    }

    if (!this->_enemy_texture->loadFromFile("assets/sprites/enemy.png")) {
        throw std::runtime_error("Could not load enemy texture");
    }

    if (!this->_bullet_texture->loadFromFile("assets/sprites/bullet.png")) {
        throw std::runtime_error("Could not load bullet texture");
    }

    this->thread = new Thread(update_scene, this);
}

Scene::~Scene() {
    if (this->_player) {
        delete this->_player;
        this->_player = nullptr;
    }

    if (this->_scene_sem) {
        delete this->_scene_sem;
        this->_scene_sem = nullptr;
    }
}

void Scene::start_game() {
    this->create_player();

    // TODO: Mudar pra 4 depois
    for (int i = 0; i < 1; i++) {
        this->create_enemy();
    }
}

void Scene::update_scene(Scene *scene) {
    while (true) {
        Game::lock_state();
        if (Game::get_state() == StateMachine::State::EXIT) {
            Game::unlock_state();
            break;
        } else if (Game::get_state() != StateMachine::State::INGAME) {
            Game::unlock_state();
            Thread::yield();
            continue;
        }
        Game::unlock_state();

        scene->lock_scene();
        // TODO: Rotina de spawn de inimigos
        Player *player = scene->get_player();
        player->lock_target_move();
        scene->solve_collisions(player);
        player->unlock_target_move();

        for (size_t i = 0; i < scene->_enemies->size(); i++) {
            Enemy *enemy = scene->_enemies->at(i);
            enemy->lock_target_move();
            scene->solve_collisions(enemy);
            enemy->unlock_target_move();
        }

        for (size_t i = 0; i < scene->_bullets->size(); i++) {
            scene->solve_collisions(scene->_bullets->at(i));
        }
        scene->unlock_scene();

        Thread::yield();
    }

    // TODO: Limpar tudo que foi alocado

    scene->get_thread()->thread_exit(0);
}

void Scene::create_player() {
    this->lock_scene();
    int x = this->_width / 2;
    int y = this->_height / 2;
    this->_player = new Player(x, y, this->_player_texture);
    this->unlock_scene();
}

void Scene::create_enemy() {
    this->lock_scene();
    this->_enemies->push_back(new Enemy(4, 4, 0, 0.0f, this->_enemy_texture));
    this->unlock_scene();
}

void Scene::create_bullet() {
    // this->_bullets.push_back(Bullet());
}

void Scene::solve_collisions(Entity *entity) {
    bool move = true;

    int target_rotation = entity->get_target_rotation();
    int direction = entity->get_target_direction();
    int new_rotation = (entity->get_rotation() + target_rotation) % 360;
    int new_x = entity->get_position()[0] + static_cast<int>(sin(entity->get_rotation() * M_PI / 180) * direction);
    int new_y = entity->get_position()[1] + static_cast<int>(cos(entity->get_rotation() * M_PI / 180) * -direction);

    if (target_rotation != 0) {
        entity->get_shape()->rotate(target_rotation > 0);
    }

    // TODO: Resolver colisões

    if (entity->get_id() != this->_player->get_id()) {
        move = solve_precise_collision(entity, this->_player, new_x, new_y, target_rotation);
    }

    for (size_t i = 0; i < this->_enemies->size(); i++) {
        if (entity->get_id() != this->_enemies->at(i)->get_id()) {
            move = solve_precise_collision(entity, this->_enemies->at(i), new_x, new_y, target_rotation);
        }
    }

    for (size_t i = 0; i < this->_bullets->size(); i++) {
        if (entity->get_id() != this->_bullets->at(i)->get_id()) {
            move = solve_precise_collision(entity, this->_bullets->at(i), new_x, new_y, target_rotation);
        }
    }

    entity->reset_target_move();

    if (move) {
        entity->set_position_and_rotation(new_x, new_y, new_rotation);
    }
}

bool Scene::solve_precise_collision(Entity *entity1, Entity *entity2, int new_x, int new_y, int rotation) {
    int distance_x = abs(entity2->get_position()[0] - new_x);
    int distance_y = abs(entity2->get_position()[1] - new_y);
    int total_width = entity1->get_shape()->get_width() * 0.5 + entity2->get_shape()->get_width() * 0.5;
    int total_height = entity1->get_shape()->get_height() * 0.5 + entity2->get_shape()->get_height() * 0.5;

    if (distance_x <= total_width && distance_y <= total_height) {
        int offset = -static_cast<int>(entity1->get_shape()->get_width() * 0.5);
        int entity_offset_x = new_x - entity2->get_position()[0];
        int entity_offset_y = new_y - entity2->get_position()[1];

        for (int i = 0; i < entity1->get_shape()->get_width(); i++) {
            for (int j = 0; j < entity1->get_shape()->get_height(); j++) {
                // TODO: Mover essa lógica para outro lugar
                if (new_x + i + offset < 0 || new_x + i + offset >= this->_width || new_y + j + offset < 0 ||
                    new_y + j + offset >= this->_height) {
                    if (rotation != 0) {
                        entity1->get_shape()->rotate(rotation < 0);
                    }
                    return false;
                }

                int entity1Type = static_cast<int>(entity1->get_shape()->get(i, j));

                int comp_pos_x = i + entity_offset_x;
                int comp_pos_y = j + entity_offset_y;

                if ((comp_pos_x >= 0 && comp_pos_x < entity1->get_shape()->get_width()) &&
                    (comp_pos_y >= 0 && comp_pos_y < entity1->get_shape()->get_height())) {
                    int entity2_type = static_cast<int>(entity2->get_shape()->get(comp_pos_x, comp_pos_y));
                    Entity::Type type = static_cast<Entity::Type>(entity1Type | entity2_type);

                    if (type == (Entity::Type::PLAYER | Entity::Type::ENEMY)) {
                        if (rotation != 0) {
                            entity1->get_shape()->rotate(rotation < 0);
                        }
                        return false;
                    }
                }
            }
        }
    }

    return true;
}

void Scene::render(sf::RenderWindow *window) {
    this->lock_scene();  // TODO: Verificar se é necessário bloquear a cena para renderizar
    if (this->_player) {
        this->_player->render(window);
    }

    for (size_t i = 0; i < this->_enemies->size(); i++) {
        this->_enemies->at(i)->render(window);
    }

    for (size_t i; i < this->_bullets->size(); i++) {
        this->_bullets->at(i)->render(window);
    }
    this->unlock_scene();
}

void Scene::handle_player_control_event(StateMachine::Event event) {
    this->lock_scene();
    if (this->_player) {
        this->_player->set_control_event(event);
    }
    this->unlock_scene();
}
