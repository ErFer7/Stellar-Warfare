#include "../include/bullet.h"

__USING_API

Bullet::Bullet(int x, int y, Type type, float speed, sf::Texture *texture) : Entity(x, y, 0, speed) {
    Type shape[] = {type};

    this->set_graphics(texture);
    this->set_shape(1, 1, shape);
}

Bullet::~Bullet() {}
