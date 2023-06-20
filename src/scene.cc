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
        scene->update_bullets_behavior();
        // TODO: Rotina de spawn de inimigos
        scene->update_all_entities();
        scene->destroy_dead_spaceships();
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

void Scene::destroy_player() {
    delete this->_player;
    this->_player = nullptr;
}

void Scene::destroy_bullet(unsigned int i) {
    delete (*this->_bullets)[i];
    (*this->_bullets)[i] = nullptr;
}

void Scene::destroy_enemy(unsigned int i) {
    delete (*this->_enemies)[i];
    (*this->_enemies)[i] = nullptr;
}

void Scene::update_all_entities() {
    // Processa o jogador
    if (this->_player) {
        this->_player->lock();
        if (this->_player->is_shooting()) {
            this->_player->reset_shooting();
            int x = this->_player->get_shot_spawn_x();
            int y = this->_player->get_shot_spawn_y();
            int rotation = this->_player->get_rotation();
            create_bullet(x, y, rotation, Entity::Type::PLAYER_BULLET);
        }

        if (this->_player->has_target_move()) {
            solve_collisions(this->_player);
        }
        this->_player->unlock();
    }

    // Processa os inimigos
    for (unsigned int i = 0; i < this->_enemies->size(); i++) {
        Enemy *enemy = (*this->_enemies)[i];
        if (enemy) {
            enemy->lock();
            if (enemy->get_health() <= 0) {
                enemy->unlock();
                continue;
            }

            if (enemy->is_shooting()) {
                enemy->reset_shooting();
                int x = enemy->get_shot_spawn_x();
                int y = enemy->get_shot_spawn_y();
                int rotation = enemy->get_rotation();
                create_bullet(x, y, rotation, Entity::Type::ENEMY_BULLET);
            }

            if (enemy->has_target_move()) {
                solve_collisions(enemy);
            }
            enemy->unlock();
        }
    }

    // Processa as balas
    for (unsigned int i = 0; i < this->_bullets->size(); i++) {
        Bullet *bullet = (*this->_bullets)[i];
        if (bullet) {
            solve_collisions(bullet);
        }
    }

    // Reseta o estado de simulação de todas as entidades
    if (this->_player) {
        this->_player->set_already_simulated(false);
    }

    for (unsigned int i = 0; i < this->_enemies->size(); i++) {
        Enemy *enemy = (*this->_enemies)[i];
        if (enemy) {
            enemy->set_already_simulated(false);
        }
    }

    for (unsigned int i = 0; i < this->_bullets->size(); i++) {
        Bullet *bullet = (*this->_bullets)[i];
        if (bullet) {
            bullet->set_already_simulated(false);
        }
    }
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

    entity->reset_target_move();

    CollisionResult result = OK;

    result = solve_boundary_collision(entity, new_x, new_y, target_rotation);

    if (result == DESTROYED) {
        apply_collision_result(result, entity, new_x, new_y, new_rotation);
        return;
    }

    if (this->_player && entity->get_id() != this->_player->get_id() && !this->_player->already_simulated()) {
        CollisionResult temp_result = check_precise_collision(entity, this->_player, new_x, new_y);
        result = temp_result != OK ? temp_result : result;

        if (result == DESTROYED) {
            apply_collision_result(result, entity, new_x, new_y, new_rotation);
            return;
        }
    }

    for (unsigned int i = 0; i < this->_enemies->size(); i++) {
        Enemy *enemy = (*this->_enemies)[i];
        if (enemy) {
            if (entity->get_id() != enemy->get_id() && !enemy->already_simulated()) {
                CollisionResult temp_result = check_precise_collision(entity, enemy, new_x, new_y);
                result = temp_result != OK ? temp_result : result;

                if (temp_result == DESTROYED) {
                    apply_collision_result(result, entity, new_x, new_y, new_rotation);
                    return;
                }
            }
        }
    }

    for (unsigned int i = 0; i < this->_bullets->size(); i++) {
        Bullet *bullet = (*this->_bullets)[i];
        if (bullet) {
            if (entity->get_id() != bullet->get_id() && !bullet->already_simulated()) {
                CollisionResult temp_result = check_precise_collision(entity, bullet, new_x, new_y);
                result = temp_result != OK ? temp_result : result;

                if (temp_result == DESTROYED) {
                    apply_collision_result(result, entity, new_x, new_y, new_rotation);
                    return;
                }
            }
        }
    }

    apply_collision_result(result, entity, new_x, new_y, new_rotation);
}

Scene::CollisionResult Scene::check_precise_collision(Entity *entity1, Entity *entity2, int new_x, int new_y) {
    int entity2_x = entity2->get_position()[0];
    int entity2_y = entity2->get_position()[1];
    int distance_x = abs(entity2_x - new_x);
    int distance_y = abs(entity2_y - new_y);
    int entity1_size = entity1->get_size();
    int entity2_size = entity2->get_size();
    int total_width = entity1_size * 0.5 + entity2_size * 0.5;
    int total_height = entity1_size * 0.5 + entity2_size * 0.5;

    if (distance_x <= total_width && distance_y <= total_height) {
        return solve_entity_collision(entity1, entity2);
    }

    return OK;
}

Scene::CollisionResult Scene::solve_boundary_collision(Entity *entity, int new_x, int new_y, int rotation) {
    int offset = -entity->get_size() * 0.5;

    for (int i = 0; i < entity->get_size(); i++) {
        for (int j = 0; j < entity->get_size(); j++) {
            int x = i + new_x + offset;
            int y = j + new_y + offset;
            if (x < 0 || x >= this->_width || y < 0 || y >= this->_height) {
                if (entity->get_type() == Entity::Type::PLAYER_BULLET ||
                    entity->get_type() == Entity::Type::ENEMY_BULLET) {
                    destroy_bullet(entity->get_index());
                    return DESTROYED;
                }
                return BLOCKED;
            }
        }
    }

    return OK;
}

Scene::CollisionResult Scene::solve_entity_collision(Entity *entity1, Entity *entity2) {
    Entity::Type entity1_type = entity1->get_type();
    Entity::Type entity2_type = entity2->get_type();

    switch (entity1_type) {
        case Entity::Type::PLAYER:
            switch (entity2_type) {
                case Entity::Type::ENEMY:
                    static_cast<Player *>(entity1)->kill();
                    static_cast<Enemy *>(entity2)->kill();
                    return DESTROYED;
                case Entity::Type::ENEMY_BULLET:
                    static_cast<Player *>(entity1)->apply_damage(1);

                    if (static_cast<Player *>(entity1)->get_health() <= 0) {
                        static_cast<Player *>(entity1)->kill();
                    }
                    destroy_bullet(entity2->get_index());
                    return OK;
                default:
                    break;
            }
            break;
        case Entity::Type::ENEMY:
            switch (entity2_type) {
                case Entity::Type::PLAYER:
                    static_cast<Player *>(entity2)->kill();
                    static_cast<Enemy *>(entity1)->kill();
                    return DESTROYED;
                case Entity::Type::ENEMY:
                    return BLOCKED;
                case Entity::Type::PLAYER_BULLET:
                    static_cast<Enemy *>(entity1)->kill();
                    destroy_bullet(entity2->get_index());
                    return DESTROYED;
                default:
                    break;
            }
            break;
        case Entity::Type::PLAYER_BULLET:
            switch (entity2_type) {
                case Entity::Type::ENEMY:
                    static_cast<Enemy *>(entity2)->kill();
                    destroy_bullet(entity1->get_index());
                    return DESTROYED;
                case Entity::Type::ENEMY_BULLET:
                    destroy_bullet(entity1->get_index());
                    destroy_bullet(entity2->get_index());
                    return DESTROYED;
                default:
                    break;
            }
            break;
        case Entity::Type::ENEMY_BULLET:
            switch (entity2_type) {
                case Entity::Type::PLAYER:
                    static_cast<Player *>(entity2)->apply_damage(1);

                    if (static_cast<Player *>(entity2)->get_health() <= 0) {
                        static_cast<Player *>(entity2)->kill();
                    }
                    destroy_bullet(entity1->get_index());
                    return DESTROYED;
                case Entity::Type::PLAYER_BULLET:
                    destroy_bullet(entity1->get_index());
                    destroy_bullet(entity2->get_index());
                    return DESTROYED;
                default:
                    break;
            }
            break;
        default:
            break;
    }

    return OK;
}

void Scene::destroy_dead_spaceships() {
    if (this->_player) {
        this->_player->lock();
        if (this->_player->get_health() <= 0) {
            this->_player->unlock();
            this->_player->join();
            destroy_player();

            // Emitir evento de fim de jogo
        } else {
            this->_player->unlock();
        }
    }

    for (unsigned int i = 0; i < this->_enemies->size(); i++) {
        Enemy *enemy = (*this->_enemies)[i];
        if (enemy) {
            enemy->lock();
            if (enemy->get_health() <= 0) {
                enemy->unlock();
                enemy->join();
                destroy_enemy(enemy->get_index());
            } else {
                enemy->unlock();
            }
        }
    }
}

void Scene::update_bullets_behavior() {
    for (unsigned int i = 0; i < this->_bullets->size(); i++) {
        Bullet *bullet = (*this->_bullets)[i];
        if (bullet) {
            bullet->update_behaviour();
        }
    }
}

void Scene::apply_collision_result(CollisionResult result, Entity *entity, int new_x, int new_y, int new_rotation) {
    entity->set_already_simulated(true);

    switch (result) {
        case OK:
            entity->set_position_and_rotation(new_x, new_y, new_rotation);
            break;
        default:
            break;
    }
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
