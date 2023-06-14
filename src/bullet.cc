#include "../include/bullet.h"

__USING_API

Bullet::Bullet(int x, int y, sf::Texture *texture) : Entity(x, y, 0.0f, texture) {}

Bullet::~Bullet() {}
