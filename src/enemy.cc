#include "../include/enemy.h"
#include <random>

__USING_API

Enemy::Enemy(int x, int y, float rotation, float speed, sf::Texture *texture)
    : Spaceship(x, y, rotation, speed) {
    Type shape[] = {VOID, ENEMY, VOID, ENEMY, ENEMY, ENEMY, ENEMY, VOID, ENEMY};

    this->set_graphics(texture);
    this->set_shape(3, 3, shape);
    this->thread = new Thread(this->update_behaviour, this);
}

Enemy::~Enemy() {}

void Enemy::update_behaviour(Enemy *enemy) {
    while (!enemy->is_destroyed()) {
        enemy->lock_target_move();
        enemy->set_target_move(rand() % 3 - 1, 0);
        enemy->unlock_target_move();

        Thread::yield();
    }

    enemy->get_thread()->thread_exit(0);
}
