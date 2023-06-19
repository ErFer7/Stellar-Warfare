#include "../include/scene.h"

#include <math.h>

#include <iostream>
#include <stdexcept>

__USING_API

Scene::Scene() {
    this->_width = 30;
    this->_height = 30;
    this->_score = 0;
    this->_player = nullptr;
    this->_enemies = new DynamicArray<Enemy *>(4);
    this->_bullets = new DynamicArray<Bullet *>(10);
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
    this->lock_scene();
    this->_score = 0;
    this->create_player();

    for (int i = 0; i < 4; i++) {
        this->create_enemy(i);
    }
    this->unlock_scene();
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
        for (unsigned int i = 0; i < scene->_bullets->size(); i++) {
            Bullet *bullet = (*scene->_bullets)[i];
            if (bullet) {
                bullet->update_behaviour();
            }
        }

        // TODO: Rotina de spawn de inimigos

        Player *player = scene->get_player();
        player->lock_action();
        if (player->is_shooting()) {
            player->reset_shooting();
            int x = player->get_shot_spawn_x();
            int y = player->get_shot_spawn_y();
            int rotation = player->get_rotation();
            scene->create_bullet(x, y, rotation, Entity::Type::PLAYER_BULLET);
        }

        if (player->has_target_move()) {
            scene->solve_collisions(player);
        }
        player->unlock_action();

        for (unsigned int i = 0; i < scene->_enemies->size(); i++) {
            Enemy *enemy = (*scene->_enemies)[i];
            if (enemy) {
                enemy->lock_action();
                if (enemy->is_shooting()) {
                    enemy->reset_shooting();
                    int x = enemy->get_shot_spawn_x();
                    int y = enemy->get_shot_spawn_y();
                    int rotation = enemy->get_rotation();
                    scene->create_bullet(x, y, rotation, Entity::Type::ENEMY_BULLET);
                }

                if (enemy->has_target_move()) {
                    scene->solve_collisions(enemy);
                }
                enemy->unlock_action();
            }
        }

        for (unsigned int i = 0; i < scene->_bullets->size(); i++) {
            Bullet *bullet = (*scene->_bullets)[i];
            if (bullet) {
                scene->solve_collisions(bullet);
            }
        }
        scene->unlock_scene();

        Thread::yield();
    }

    // TODO: Limpar tudo que foi alocado

    scene->get_thread()->thread_exit(0);
}

void Scene::create_player() {
    int x = this->_width / 2;
    int y = this->_height / 2;
    this->_player = new Player(x, y, this->_player_texture);
}

void Scene::create_enemy(int spot) {
    if (spot == -1) {
        spot = rand() % 4;
    }

    int spawn_x;
    int spawn_y;
    int spawn_rotation;

    switch (spot) {
        case 0:
            spawn_x = 2;
            spawn_y = 2;
            spawn_rotation = 180;
            break;
        case 1:
            spawn_x = 2;
            spawn_y = this->_height - 3;
            spawn_rotation = 0;
            break;
        case 2:
            spawn_x = this->_width - 3;
            spawn_y = this->_height - 3;
            spawn_rotation = 0;
            break;
        case 3:
            spawn_x = this->_width - 3;
            spawn_y = 2;
            spawn_rotation = 180;
            break;
        default:
            break;
    }

    // TODO: Checar se o spot está ocupado

    unsigned int index = this->_enemies->add(new Enemy(spawn_x, spawn_y, spawn_rotation, 8.0f, this->_enemy_texture));
    (*this->_enemies)[index]->set_index(index);
}

void Scene::create_bullet(int x, int y, int rotation, Entity::Type type) {
    unsigned int index = this->_bullets->add(new Bullet(x, y, rotation, type, this->_bullet_texture));
    (*this->_bullets)[index]->set_index(index);
}

void Scene::destroy_bullet(unsigned int i) {
    delete (*this->_bullets)[i];
    (*this->_bullets)[i] = nullptr;
}

void Scene::solve_collisions(Entity *entity) {
    int target_rotation = entity->get_target_rotation();
    int direction = entity->get_target_direction();
    int old_rotation = entity->get_rotation();
    int new_rotation = (old_rotation + target_rotation) % 360;
    int old_x = entity->get_position()[0];
    int old_y = entity->get_position()[1];
    int new_x = old_x + static_cast<int>(sin(old_rotation * M_PI / 180) * direction);
    int new_y = old_y + static_cast<int>(cos(old_rotation * M_PI / 180) * -direction);

    if (target_rotation != 0) {
        entity->get_shape()->rotate(target_rotation > 0);
    }

    entity->reset_target_move();

    if (!solve_boundary_collision(entity, new_x, new_y, target_rotation)) {
        return;
    }

    if (entity->get_id() != this->_player->get_id()) {
        if (!solve_precise_collision(entity, this->_player, new_x, new_y, target_rotation)) {
            return;
        }
    }

    for (unsigned int i = 0; i < this->_enemies->size(); i++) {
        Enemy *enemy = (*this->_enemies)[i];
        if (enemy) {
            if (entity->get_id() != enemy->get_id()) {
                if (!solve_precise_collision(entity, enemy, new_x, new_y, target_rotation)) {
                    return;
                }
            }
        }
    }

    for (unsigned int i = 0; i < this->_bullets->size(); i++) {
        Bullet *bullet = (*this->_bullets)[i];
        if (bullet) {
            if (entity->get_id() != bullet->get_id()) {
                if (!solve_precise_collision(entity, bullet, new_x, new_y, target_rotation)) {
                    return;
                }
            }
        }
    }

    entity->set_position_and_rotation(new_x, new_y, new_rotation);
}

bool Scene::solve_precise_collision(Entity *entity1, Entity *entity2, int new_x, int new_y, int rotation) {
    int entity2_x = entity2->get_position()[0];
    int entity2_y = entity2->get_position()[1];
    int distance_x = abs(entity2_x - new_x);
    int distance_y = abs(entity2_y - new_y);
    Matrix<bool> *entity1_shape = entity1->get_shape();
    Matrix<bool> *entity2_shape = entity2->get_shape();
    Entity::Type entity1_type = entity1->get_type();
    Entity::Type entity2_type = entity2->get_type();
    int total_width = entity1_shape->get_width() * 0.5 + entity2_shape->get_width() * 0.5;
    int total_height = entity1_shape->get_height() * 0.5 + entity2_shape->get_height() * 0.5;

    if (distance_x <= total_width && distance_y <= total_height) {
        int entity_offset_x = new_x - entity2_x;
        int entity_offset_y = new_y - entity2_y;

        for (int i = 0; i < entity1_shape->get_width(); i++) {
            for (int j = 0; j < entity1_shape->get_height(); j++) {
                bool entity1Cell = entity1_shape->get(i, j);

                int comp_pos_x = i + entity_offset_x;
                int comp_pos_y = j + entity_offset_y;

                if (comp_pos_x < 0 || comp_pos_x >= entity2_shape->get_width() || comp_pos_y < 0 ||
                    comp_pos_y >= entity2_shape->get_height()) {
                    continue;
                }

                int entity2Cell = entity2_shape->get(comp_pos_x, comp_pos_y);

                if (entity1Cell && entity2Cell) {
                    // TODO: Tratar colisões
                    if ((entity1_type == Entity::Type::PLAYER && entity2_type == Entity::Type::ENEMY) ||
                        (entity1_type == Entity::Type::ENEMY && entity2_type == Entity::Type::PLAYER)) {
                        // TODO: Destruir o jogador e terminar o jogo
                    } else if (entity1_type == Entity::Type::ENEMY && entity2_type == Entity::Type::ENEMY) {
                        if (rotation != 0) {
                            entity1_shape->rotate(rotation < 0);
                        }
                        return false;
                    } else if ((entity1_type == Entity::Type::PLAYER_BULLET &&
                                entity2_type == Entity::Type::ENEMY_BULLET) ||
                               (entity1_type == Entity::Type::ENEMY_BULLET &&
                                entity2_type == Entity::Type::PLAYER_BULLET)) {
                        // TODO: Destruir as duas balas
                        destroy_bullet(entity1->get_index());
                        destroy_bullet(entity2->get_index());
                        return false;
                    }
                }
            }
        }
    }

    return true;
}

bool Scene::solve_boundary_collision(Entity *entity, int new_x, int new_y, int rotation) {
    int offset = -static_cast<int>(entity->get_shape()->get_width() * 0.5);

    for (int i = 0; i < entity->get_shape()->get_width(); i++) {
        for (int j = 0; j < entity->get_shape()->get_height(); j++) {
            int x = i + new_x + offset;
            int y = j + new_y + offset;
            if (x < 0 || x >= this->_width || y < 0 || y >= this->_height) {
                if (entity->get_type() == Entity::Type::PLAYER_BULLET ||
                    entity->get_type() == Entity::Type::ENEMY_BULLET) {
                    destroy_bullet(entity->get_index());
                    return false;
                }

                if (rotation != 0) {
                    entity->get_shape()->rotate(rotation < 0);
                }
                return false;
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

    for (unsigned int i = 0; i < this->_enemies->size(); i++) {
        Enemy *enemy = (*this->_enemies)[i];
        if (enemy) {
            enemy->render(window);
        }
    }

    for (unsigned int i = 0; i < this->_bullets->size(); i++) {
        Bullet *bullet = (*this->_bullets)[i];
        if (bullet) {
            bullet->render(window);
        }
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
