#include "../include/bullet.h"

__USING_API

Bullet::Bullet(int x, int y, int rotation, Type type, sf::Texture *texture) : Entity(x, y, rotation, 64.0f, type, 1) {
    this->set_graphics(texture);
}

void Bullet::update_behaviour() {
    if (this->can_move()) {
        this->set_target_move(1, 0);
    }
}
