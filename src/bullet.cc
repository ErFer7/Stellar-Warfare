#include "../include/bullet.h"

__USING_API

Bullet::Bullet(int x, int y, int rotation, Type type, sf::Texture *texture) : Entity(x, y, rotation, 32.0f, type, 1) {
    this->set_graphics(texture, 1.0f);
}

void Bullet::update_behaviour() {
    if (this->can_move()) {
        int x = 0;
        int y = 0;
        int rotation = this->get_rotation();

        switch (rotation)
        {
        case 0:
            y = -1;
            break;
        case 90:
            x = 1;
            break;
        case 180:
            y = 1;
            break;
        case 270:
            x = -1;
            break;
        default:
            break;
        }

        this->set_target_move(x, y);
    }
}
