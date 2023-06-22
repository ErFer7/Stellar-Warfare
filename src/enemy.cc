#include "../include/enemy.h"

#include <random>

__USING_API

Enemy::Enemy(int x, int y, int rotation, float speed, sf::Texture *texture, float scale, int scene_offset_x,
             int scene_offset_y)
    : Spaceship(x, y, rotation, speed, ENEMY, 1, 1.5f, scale, scene_offset_x, scene_offset_y) {
    this->_algorithm = random() % 5;
    this->_last_player_position[0] = 0;
    this->_last_player_position[1] = 0;
    this->_last_move[0] = 0;
    this->_last_move[1] = 0;
    this->_successfully_moved = false;
    this->set_graphics(texture);
    this->thread = new Thread(this->update_behaviour, this);
}

Enemy::~Enemy() {}

void Enemy::update_behaviour(Enemy *enemy) {
    while (true) {
        enemy->lock();
        if (enemy->get_health() <= 0) {
            enemy->unlock();
            break;
        }

        switch (enemy->get_algorithm()) {
            case 0:
                enemy->random_alg();
                break;
            case 1:
                enemy->spinner_alg();
                break;
            case 2:
                enemy->walker_alg();
                break;
            case 3:
                enemy->hunter_alg();
                break;
            case 4:
                enemy->shooter_alg();
                break;
            default:
                break;
        }

        enemy->unlock();

        Thread::yield();
    }

    enemy->get_thread()->thread_exit(0);
}

void Enemy::tell_result(int player_x, int player_y, bool successfully_moved) {
    this->_last_player_position[0] = player_x;
    this->_last_player_position[1] = player_y;
    this->_successfully_moved = successfully_moved;
}

void Enemy::set_last_move(int x, int y) {
    this->_last_move[0] = x;
    this->_last_move[1] = y;
}

void Enemy::random_alg() {
    if (this->can_move()) {
        int random_direction = random() % 3 - 1;

        if (random() % 2) {
            this->set_target_move(random_direction, 0);
            this->set_last_move(random_direction, 0);
        } else {
            this->set_target_move(0, random_direction);
            this->set_last_move(random_direction, 0);
        }
    }

    if (this->can_shoot()) {
        if (random() % 64 == 0) {
            this->shoot();
        }
    }
}

void Enemy::spinner_alg() {
    if (this->can_move()) {
        int x = 0;
        int y = 0;

        switch (this->get_rotation()) {
            case 0:
                x = 1;
                break;
            case 90:
                y = 1;
                break;
            case 180:
                x = -1;
                break;
            case 270:
                y = -1;
                break;
            default:
                break;
        }

        this->set_target_move(x, y);
        this->set_last_move(x, y);
    }

    if (this->can_shoot()) {
        if (random() % 2) {
            this->shoot();
        }
    }
}

void Enemy::walker_alg() {
    if (this->can_move()) {
        int x = this->get_last_move()[0];
        int y = this->get_last_move()[1];

        if (this->successfully_moved() && (x != 0 || y != 0)) {
            this->set_target_move(x, y);
        } else {
            int random_direction = random() % 3 - 1;

            if (x != 0) {
                if (random() % 2) {
                    x = -x;
                    y = 0;
                } else {
                    x = 0;
                    y = random_direction;
                }
            } else if (y != 0) {
                if (random() % 2) {
                    x = 0;
                    y = -y;
                } else {
                    x = random_direction;
                    y = 0;
                }
            } else {
                if (random() % 2) {
                    x = random_direction;
                    y = 0;
                } else {
                    x = 0;
                    y = random_direction;
                }
            }

            this->set_target_move(x, y);
            this->set_last_move(x, y);
        }
    }

    if (this->can_shoot()) {
        if (random() % 256 == 0) {
            this->shoot();
        }
    }
}

void Enemy::hunter_alg() {
    if (this->can_move()) {
        int x = 0;
        int y = 0;

        int pos_x = this->get_position()[0];
        int pos_y = this->get_position()[1];

        int player_x = this->get_last_player_position()[0];
        int player_y = this->get_last_player_position()[1];

        if (player_x > pos_x) {
            x = 1;
        } else if (player_x < pos_x) {
            x = -1;
        }

        if (player_y > pos_y) {
            y = 1;
        } else if (player_y < pos_y) {
            y = -1;
        }

        if (x != 0 && y != 0) {
            if (random() % 2) {
                x = 0;
            } else {
                y = 0;
            }
        }

        this->set_target_move(x, y);
        this->set_last_move(x, y);
    }
}

void Enemy::shooter_alg() {
    int pos_x = this->get_position()[0];
    int pos_y = this->get_position()[1];

    int player_x = this->get_last_player_position()[0];
    int player_y = this->get_last_player_position()[1];

    if (this->can_move()) {
        int x = 0;
        int y = 0;

        if (player_x > pos_x) {
            x = 1;
        } else if (player_x < pos_x) {
            x = -1;
        } else if (this->get_rotation() == 270 || this->get_rotation() == 90) {
            if (player_y > pos_y) {
                y = 1;
            } else if (player_y < pos_y) {
                y = -1;
            }
        }

        this->set_target_move(x, y);
        this->set_last_move(x, y);
    }

    if (this->can_shoot()) {
        if (pos_x == player_x || pos_y == player_y) {
            this->shoot();
        }
    }
}
