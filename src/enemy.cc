#include "../include/enemy.h"

#include <random>

__USING_API

Enemy::Enemy(int x, int y, float rotation, float speed, sf::Texture *texture)
    : Spaceship(x, y, rotation, speed, ENEMY, 1) {
    bool shape[] = {false, true, false, true, true, true, true, false, true};

    this->set_graphics(texture);
    this->set_shape(3, 3, shape);
    this->thread = new Thread(this->update_behaviour, this);
}

Enemy::~Enemy() {}

void Enemy::update_behaviour(Enemy *enemy) {
    while (!enemy->is_destroyed()) {
        if (!enemy->can_move()) {
            Thread::yield();
            continue;
        }

        enemy->lock_action();
        int random_direction = rand() % 3 - 1;
        if (random() % 2) {
            enemy->set_target_move(random_direction, 90);
        } else {
            enemy->set_target_move(random_direction, -90);
        }

        if (random() % 30 == 0) {
            enemy->shoot();
        }

        enemy->unlock_action();

        Thread::yield();
    }

    enemy->get_thread()->thread_exit(0);
}
