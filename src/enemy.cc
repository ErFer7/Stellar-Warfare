#include "../include/enemy.h"

#include <random>

__USING_API

Enemy::Enemy(int x, int y, int rotation, float speed, sf::Texture *texture, float scale, int scene_offset_x, int scene_offset_y)
    : Spaceship(x, y, rotation, speed, ENEMY, 1, 1.5f, scale, scene_offset_x, scene_offset_y) {
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

        if (enemy->can_move()) {
            switch (random() % 4) {
                case 0:
                    enemy->set_target_move(0, -1);
                    break;
                case 1:
                    enemy->set_target_move(0, 1);
                    break;
                case 2:
                    enemy->set_target_move(1, 0);
                    break;
                case 3:
                    enemy->set_target_move(-1, 0);
                    break;
                default:
                    break;
            }
        }

        if (enemy->can_shoot()) {
            if (random() % 10 == 0) {
                enemy->shoot();
            }
        }
        enemy->unlock();

        Thread::yield();
    }

    enemy->get_thread()->thread_exit(0);
}
