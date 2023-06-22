#include "../include/enemy.h"

#include <random>

__USING_API

Enemy::Enemy(int x, int y, int rotation, float speed, sf::Texture *texture, float scale, int scene_offset_x,
             int scene_offset_y)
    : Spaceship(x, y, rotation, speed, ENEMY, 1, 1.5f, scale, scene_offset_x, scene_offset_y) {
    this->_algorithm = random() % 2;
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
            default:
                break;
        }

        enemy->unlock();

        Thread::yield();
    }

    enemy->get_thread()->thread_exit(0);
}

void Enemy::random_alg() {
    if (this->can_move()) {
        switch (random() % 4) {
            case 0:
                this->set_target_move(0, -1);
                break;
            case 1:
                this->set_target_move(0, 1);
                break;
            case 2:
                this->set_target_move(1, 0);
                break;
            case 3:
                this->set_target_move(-1, 0);
                break;
            default:
                break;
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
        switch (this->get_rotation()) {
            case 0:
                this->set_target_move(1, 0);
                break;
            case 90:
                this->set_target_move(0, 1);
                break;
            case 180:
                this->set_target_move(-1, 0);
                break;
            case 270:
                this->set_target_move(0, -1);
                break;
            default:
                break;
        }
    }

    if (this->can_shoot()) {
        this->shoot();
    }
}
