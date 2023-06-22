#include "../include/scene.h"

#include <math.h>

#include <iostream>
#include <random>
#include <stdexcept>

__USING_API

Scene::Scene() {
    this->_width = 30;
    this->_height = 30;
    this->_scale = 1.5f;
    this->_scene_offset[0] = 24;
    this->_scene_offset[1] = 24;
    this->_score = 0;
    this->_player = nullptr;
    this->_skip_time = false;
    this->_enemy_spawn_count = 4;
    this->_enemy_kill_count = 0;
    this->_level = 1;
    this->_scene_sem = new Semaphore(1);
    this->_enemies = new DynamicArray<Enemy *>(4, nullptr);
    this->_bullets = new DynamicArray<Bullet *>(10, nullptr);
    this->_enemy_spawn_times = new DynamicArray<float>(4, -1.0f);
    this->_enemy_spawn_times->fill(-1.0f);
    this->_internal_state = StateMachine::State::NONINITIALIZED;

    this->_clock = new sf::Clock();
    this->_player_texture = new sf::Texture();
    this->_enemy_texture = new sf::Texture();
    this->_cell_texture = new sf::Texture();
    this->_background_cell = new sf::Sprite();

    if (!this->_player_texture->loadFromFile("assets/sprites/player.png")) {
        throw std::runtime_error("Could not load player texture");
    }

    if (!this->_enemy_texture->loadFromFile("assets/sprites/enemy.png")) {
        throw std::runtime_error("Could not load enemy texture");
    }

    if (!this->_cell_texture->loadFromFile("assets/sprites/cell.png")) {
        throw std::runtime_error("Could not load cell texture");
    }

    this->_background_cell->setTexture(*this->_cell_texture);
    this->_background_cell->setScale(this->_scale, this->_scale);

    this->thread = new Thread(update_scene, this);
}

Scene::~Scene() {
    if (this->_player) {
        delete this->_player;
    }

    delete this->_scene_sem;
    delete this->_enemies;
    delete this->_bullets;
    delete this->_enemy_spawn_times;
    delete this->_clock;
    delete this->_player_texture;
    delete this->_enemy_texture;
    delete this->_cell_texture;
    delete this->_background_cell;
}

void Scene::start_game() {
    this->_score = 0;
    this->_level = 1;
    this->_enemy_spawn_count = 4;
    this->_enemy_kill_count = 0;
    this->_enemy_spawn_times->fill(-1.0f);

    this->create_player();

    for (int i = 0; i < 4; i++) {
        this->create_enemy(i);
    }
}

void Scene::end_game() {
    if (this->_player) {
        this->_player->lock();
        this->destroy_player();
    }

    for (unsigned int i = 0; i < this->_enemies->size(); i++) {
        Enemy *enemy = (*this->_enemies)[i];

        if (enemy) {
            enemy->lock();
            this->destroy_enemy(i);
        }
    }

    for (unsigned int i = 0; i < this->_bullets->size(); i++) {
        Bullet *bullet = (*this->_bullets)[i];

        if (bullet) {
            this->destroy_bullet(i);
        }
    }
}

void Scene::update_enemies_speed() {
    if (this->_enemy_kill_count >= 4) {
        this->_enemy_kill_count = 0;
        this->_level++;
    }

    for (unsigned int i = 0; i < this->_enemies->size(); i++) {
        Enemy *enemy = (*this->_enemies)[i];

        if (enemy) {
            enemy->lock();
            enemy->set_speed(this->level_speed());
            enemy->unlock();
        }
    }
}

void Scene::render_background(sf::RenderWindow *window, int noise_range) {
    for (int i = 0; i < this->_height; i++) {
        for (int j = 0; j < this->_width; j++) {
            int noise = random() % (int)noise_range - ceil(noise_range * 0.5f);

            float width = this->_background_cell->getLocalBounds().width;
            float height = this->_background_cell->getLocalBounds().height;
            int x = j * width * this->_scale + this->_scene_offset[0];
            int y = i * height * this->_scale + this->_scene_offset[1];

            this->_background_cell->setColor(sf::Color(8 + noise, 24 + noise, 32 + noise, 255));
            this->_background_cell->setPosition(x, y);

            window->draw(*this->_background_cell);
        }
    }
}

void Scene::update_scene(Scene *scene) {
    while (true) {
        scene->lock_scene();
        if (scene->get_internal_state() == StateMachine::State::EXIT) {
            scene->unlock_scene();
            break;
        } else if (scene->get_internal_state() != StateMachine::State::INGAME) {
            scene->unlock_scene();
            Thread::yield();
            continue;
        }
        scene->unlock_scene();

        scene->update_bullets_behavior();

        scene->lock_scene();
        scene->spawn_enemies();
        scene->update_all_entities();
        scene->update_enemies_speed();
        scene->unlock_scene();

        Thread::yield();
    }

    scene->lock_scene();
    scene->end_game();
    scene->unlock_scene();

    scene->get_thread()->thread_exit(0);
}

void Scene::create_player() {
    int x = this->_width / 2;
    int y = this->_height / 2;

    this->_player =
        new Player(x, y, this->_player_texture, this->_scale, this->_scene_offset[0], this->_scene_offset[1]);
}

void Scene::create_enemy(int spot) {
    int spawn_x;
    int spawn_y;
    int spawn_rotation;

    if (spot == -1) {
        spot = rand() % 4;
    }

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

    if (this->_player) {
        int x = this->_player->get_position()[0];
        int y = this->_player->get_position()[1];

        if (!check_corner_collision(spawn_x, spawn_y, x, y, 3, 3)) {
            return;
        }
    }

    for (unsigned int i = 0; i < this->_enemies->size(); i++) {
        Enemy *enemy = (*this->_enemies)[i];
        if (enemy) {
            int x = enemy->get_position()[0];
            int y = enemy->get_position()[1];

            if (!check_corner_collision(spawn_x, spawn_y, x, y, 3, 3)) {
                return;
            }
        }
    }

    for (unsigned int i = 0; i < this->_bullets->size(); i++) {
        Bullet *bullet = (*this->_bullets)[i];
        if (bullet) {
            int x = bullet->get_position()[0];
            int y = bullet->get_position()[1];

            if (!check_corner_collision(spawn_x, spawn_y, x, y, 3, 3)) {
                return;
            }
        }
    }

    unsigned int index = this->_enemies->add(new Enemy(spawn_x, spawn_y, spawn_rotation, this->level_speed(), this->_enemy_texture,
                                      this->_scale, this->_scene_offset[0], this->_scene_offset[1]));
    (*this->_enemies)[index]->set_index(index);
    this->_enemy_spawn_count--;
}

void Scene::create_bullet(int x, int y, int rotation, Entity::Type type) {
    unsigned int index = this->_bullets->add(new Bullet(x, y, rotation, type, this->_cell_texture, this->_scale, this->_scene_offset[0], this->_scene_offset[1]));
    (*this->_bullets)[index]->set_index(index);
}

void Scene::destroy_player() {
    this->_player->kill();
    this->_player->unlock();
    this->_player->join();
    delete this->_player;
    this->_player = nullptr;
}

void Scene::destroy_bullet(unsigned int i) {
    delete (*this->_bullets)[i];
    (*this->_bullets)[i] = nullptr;
}

void Scene::destroy_enemy(unsigned int i) {
    (*this->_enemies)[i]->kill();
    (*this->_enemies)[i]->unlock();
    (*this->_enemies)[i]->join();
    delete (*this->_enemies)[i];
    (*this->_enemies)[i] = nullptr;
    this->_enemy_spawn_times->add(2.0f);
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

        // É necessário checar se o jogador ainda existe, pois ele pode ter sido destruído
        if (this->_player) {
            this->_player->unlock();
        }
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

            // A verificação é feita assim pois o ponteiro enemy pode não ser nulo após a destruição
            if ((*this->_enemies)[i]) {
                enemy->unlock();
            }
        }
    }

    // Processa as balas
    for (unsigned int i = 0; i < this->_bullets->size(); i++) {
        Bullet *bullet = (*this->_bullets)[i];
        if (bullet) {
            solve_collisions(bullet);
        }
    }
}

void Scene::solve_collisions(Entity *entity) {
    int new_rotation = entity->get_rotation();
    int target_move_x = entity->get_target_move()[0];
    int target_move_y = entity->get_target_move()[1];
    int new_x = entity->get_position()[0] + target_move_x;
    int new_y = entity->get_position()[1] + target_move_y;

    if (target_move_x == 1) {
        new_rotation = 90;
    } else if (target_move_x == -1) {
        new_rotation = 270;
    } else if (target_move_y == -1) {
        new_rotation = 0;
    } else if (target_move_y == 1) {
        new_rotation = 180;
    }

    entity->reset_target_move();

    if (!solve_boundary_collision(entity, new_x, new_y)) {
        return;
    }

    if (this->_player && entity->get_id() != this->_player->get_id()) {
        if (!check_precise_collision(entity, this->_player, new_x, new_y)) {
            return;
        }
    }

    for (unsigned int i = 0; i < this->_enemies->size(); i++) {
        Enemy *enemy = (*this->_enemies)[i];
        if (enemy && entity->get_id() != enemy->get_id()) {
            if (!check_precise_collision(entity, enemy, new_x, new_y)) {
                return;
            }
        }
    }

    for (unsigned int i = 0; i < this->_bullets->size(); i++) {
        Bullet *bullet = (*this->_bullets)[i];
        if (bullet && entity->get_id() != bullet->get_id()) {
            if (!check_precise_collision(entity, bullet, new_x, new_y)) {
                return;
            }
        }
    }

    entity->set_position_and_rotation(new_x, new_y, new_rotation);
}

bool Scene::check_precise_collision(Entity *entity1, Entity *entity2, int new_x, int new_y) {
    int x2 = entity2->get_position()[0];
    int y2 = entity2->get_position()[1];
    int size1 = entity1->get_size();
    int size2 = entity2->get_size();

    if (entity1->get_size() >= entity2->get_size()) {
        if (!check_corner_collision(new_x, new_y, x2, y2, size1, size2)) {
            return solve_entity_collision(entity1, entity2);
        }
    } else {
        if (!check_corner_collision(x2, y2, new_x, new_y, size2, size1)) {
            return solve_entity_collision(entity1, entity2);
        }
    }

    return true;
}

// Verifica se uma entidade está dentro da outra
bool Scene::check_corner_collision(int x1, int y1, int x2, int y2, int size1, int size2) {
    int offset1 = size1 * 0.5f;
    int offset2 = size2 * 0.5f;

    // Maior entidade (ou igual)
    int entity1_right = x1 + offset1;
    int entity1_left = x1 - offset1;
    int entity1_top = y1 - offset1;
    int entity1_bottom = y1 + offset1;

    // Menor entidade (ou igual)
    int entity2_right = x2 + offset2;
    int entity2_left = x2 - offset2;
    int entity2_top = y2 - offset2;
    int entity2_bottom = y2 + offset2;

    bool horizontal_check_left = entity2_left >= entity1_left && entity2_left <= entity1_right;
    bool horizontal_check_right = entity2_right >= entity1_left && entity2_right <= entity1_right;
    bool vertical_check_top = entity2_top >= entity1_top && entity2_top <= entity1_bottom;
    bool vertical_check_bottom = entity2_bottom >= entity1_top && entity2_bottom <= entity1_bottom;

    bool top_left = horizontal_check_left && vertical_check_top;
    bool top_right = horizontal_check_right && vertical_check_top;
    bool bottom_left = horizontal_check_left && vertical_check_bottom;
    bool bottom_right = horizontal_check_right && vertical_check_bottom;

    return !(top_left || top_right || bottom_left || bottom_right);
}

bool Scene::solve_boundary_collision(Entity *entity, int new_x, int new_y) {
    int offset = entity->get_size() * 0.5f;
    int right = new_x + offset;
    int left = new_x - offset;
    int top = new_y - offset;
    int bottom = new_y + offset;

    if (left < 0 || right >= this->_width || top < 0 || bottom >= this->_height) {
        if (entity->get_type() == Entity::Type::PLAYER_BULLET || entity->get_type() == Entity::Type::ENEMY_BULLET) {
            destroy_bullet(entity->get_index());
        }

        return false;
    }

    return true;
}

bool Scene::solve_entity_collision(Entity *entity1, Entity *entity2) {
    Entity::Type entity1_type = entity1->get_type();
    Entity::Type entity2_type = entity2->get_type();
    Player *player = nullptr;
    Enemy *enemy = nullptr;

    switch (entity1_type) {
        case Entity::Type::PLAYER:
            if (entity2_type == Entity::Type::ENEMY) {
                enemy = static_cast<Enemy *>(entity2);
                enemy->lock();
                destroy_enemy(enemy->get_index());
                destroy_player();
                this->unlock_scene();
                Game::handle_event(StateMachine::Event::PLAYER_DEATH);
                return false;
            } else if (entity2_type == Entity::Type::ENEMY_BULLET) {
                // TODO: Resolver deadlock
                destroy_bullet(entity2->get_index());
                player = static_cast<Player *>(entity1);
                player->apply_damage(1);

                if (player->get_health() <= 0) {
                    destroy_player();
                    this->unlock_scene();
                    Game::handle_event(StateMachine::Event::PLAYER_DEATH);
                    return false;
                }
            }
            break;
        case Entity::Type::ENEMY:
            if (entity2_type == Entity::Type::PLAYER) {
                destroy_enemy(entity1->get_index());
                static_cast<Player *>(entity2)->lock();
                destroy_player();
                this->unlock_scene();
                Game::handle_event(StateMachine::Event::PLAYER_DEATH);
                return false;
            } else if (entity2_type == Entity::Type::ENEMY) {
                return false;
            } else if (entity2_type == Entity::Type::PLAYER_BULLET) {
                destroy_enemy(entity1->get_index());
                destroy_bullet(entity2->get_index());
                this->_score += 100;
                this->_enemy_kill_count++;
                return false;
            }
            break;
        case Entity::Type::PLAYER_BULLET:
            if (entity2_type == Entity::Type::ENEMY) {
                destroy_bullet(entity1->get_index());
                enemy = static_cast<Enemy *>(entity2);
                enemy->lock();
                destroy_enemy(enemy->get_index());
                this->_score += 100;
                this->_enemy_kill_count++;
                return false;
            } else if (entity2_type == Entity::Type::ENEMY_BULLET) {
                destroy_bullet(entity1->get_index());
                destroy_bullet(entity2->get_index());
                return false;
            }
            break;
        case Entity::Type::ENEMY_BULLET:
            if (entity2_type == Entity::Type::PLAYER) {
                destroy_bullet(entity1->get_index());
                player = static_cast<Player *>(entity2);
                player->lock();
                player->apply_damage(1);

                if (player->get_health() <= 0) {
                    destroy_player();
                    this->unlock_scene();
                    Game::handle_event(StateMachine::Event::PLAYER_DEATH);
                } else {
                    player->unlock();
                }
                return false;
            } else if (entity2_type == Entity::Type::PLAYER_BULLET) {
                destroy_bullet(entity1->get_index());
                destroy_bullet(entity2->get_index());
                return false;
            }
            break;
        default:
            break;
    }

    return true;
}

void Scene::update_bullets_behavior() {
    for (unsigned int i = 0; i < this->_bullets->size(); i++) {
        Bullet *bullet = (*this->_bullets)[i];
        if (bullet) {
            bullet->update_behaviour();
        }
    }
}

void Scene::spawn_enemies() {
    for (unsigned int i = 0; i < this->_enemy_spawn_times->size(); i++) {
        if (this->_skip_time) {
            this->_skip_time = false;
            break;
        }

        float time = (*this->_enemy_spawn_times)[i];

        if (time > 0.0f) {
            time -= this->_clock->getElapsedTime().asSeconds();

            if (time <= 0.0f) {
                this->_enemy_spawn_count++;
                time = -1.0f;
            }

            (*this->_enemy_spawn_times)[i] = time;
        }
    }

    for (int i = 0; i < this->_enemy_spawn_count; i++) {
        create_enemy();
    }

    this->_clock->restart();
}

void Scene::handle_event(StateMachine::Event event) {
    this->lock_scene();
    switch (event) {
        case StateMachine::Event::P_KEY:
            if (this->_internal_state == StateMachine::State::NONINITIALIZED) {
                this->start_game();
                this->_internal_state = StateMachine::State::INGAME;
            } else if (this->_internal_state != StateMachine::State::GAMEOVER) {
                if (this->_internal_state == StateMachine::State::PAUSED) {
                    this->_internal_state = StateMachine::State::INGAME;
                } else {
                    this->_skip_time = true;
                    this->_internal_state = StateMachine::State::PAUSED;
                }
            }
            break;
        case StateMachine::Event::R_KEY:
            if (this->_internal_state == StateMachine::State::GAMEOVER) {
                this->start_game();
                this->_internal_state = StateMachine::State::INGAME;
            }
            break;
        case StateMachine::Event::PLAYER_DEATH:
            this->end_game();
            this->_internal_state = StateMachine::State::GAMEOVER;
            break;
        case StateMachine::Event::Q_KEY:
            this->end_game();
            this->_internal_state = StateMachine::State::EXIT;
            break;
        default:
            if (this->_player) {
                this->_player->set_control_event(event);
            }
            break;
    }

    this->unlock_scene();
}

void Scene::render(sf::RenderWindow *window) {
    this->lock_scene();  // TODO: Verificar se é necessário bloquear a cena para renderizar
    this->render_background(window, 5);

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
