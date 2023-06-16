#include "../include/enemy.h"

__USING_API

Enemy::Enemy(int x, int y, float rotation, sf::Texture *texture, sf::Color color, float scale, float speed)
    : Spaceship(x, y, rotation, texture, color, scale, speed) {

        Type shape[] = {VOID, ENEMY, VOID,
                        ENEMY, ENEMY, ENEMY,
                        ENEMY, VOID, ENEMY};

        this->set_shape(3, 3, shape);
        this->thread = new Thread(this->update_behaviour, this);
    }

Enemy::~Enemy() {}

void Enemy::update_behaviour(Enemy *Enemy) {
    while (Enemy->get_health() > 0) {
        Thread::yield();
    }

    Enemy->get_thread()->thread_exit(0);
}
