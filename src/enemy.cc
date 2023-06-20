#include "../include/enemy.h"

#include <random>

__USING_API

Enemy::Enemy(int x, int y, float rotation, float speed, sf::Texture *texture)
    : Spaceship(x, y, rotation, speed, ENEMY, 1) {
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

        if (!enemy->can_move()) {
            enemy->unlock();
            Thread::yield();
            continue;
        }

        int random_direction = rand() % 3 - 1;
        if (random() % 2) {
            enemy->set_target_move(random_direction, 90);
        } else {
            enemy->set_target_move(random_direction, -90);
        }

        if (random() % 10 == 0) {
            enemy->shoot();
        }
        enemy->unlock();

        Thread::yield();
    }

    enemy->get_thread()->thread_exit(0);
}
