#include "../include/spaceship.h"

__USING_API

Spaceship::Spaceship(int x, int y, float rotation, sf::Texture *texture, sf::Color color, float scale, float speed)
    : Entity(x, y, rotation, speed, scale) {
    this->set_graphics(texture, color);
    this->thread = nullptr;
    this->_health = 1;
}

Spaceship::~Spaceship() {
    if (this->thread) {
        delete this->thread;
        this->thread = nullptr;
    }
}
