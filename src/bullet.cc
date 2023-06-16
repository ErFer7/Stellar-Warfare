#include "../include/bullet.h"

__USING_API

Bullet::Bullet(int x, int y, sf::Texture *texture, sf::Color color, float scale, Type type, float speed)
    : Entity(x, y, 0.0f, speed, scale) {
        this->set_graphics(texture, color);
        Type shape[] = {type};
        this->set_shape(1, 1, shape);
    }

Bullet::~Bullet() {}
