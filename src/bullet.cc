#include "../include/bullet.h"

__USING_API

Bullet::Bullet(int x, int y, int rotation, Type type, sf::Texture *texture)
    : Entity(x, y, rotation, 16.0f, type) {
    bool shape[] = {true};

    this->set_graphics(texture);
    this->set_shape(1, 1, shape);
}

void Bullet::update_behaviour() {
    if (this->can_move()) {
        this->set_target_move(1, 0);
    }
}
