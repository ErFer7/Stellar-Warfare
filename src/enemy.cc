#include "../include/enemy.h"

__USING_API

Enemy::Enemy(int x, int y, float rotation, sf::Texture *texture) : Spaceship(x, y, rotation, texture) {}

Enemy::~Enemy() {}
