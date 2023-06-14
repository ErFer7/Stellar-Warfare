#include "../include/spaceship.h"

__USING_API

Spaceship::Spaceship(int x, int y, float rotation, sf::Texture *texture) : Entity(x, y, rotation, texture) {}

Spaceship::~Spaceship() {}
