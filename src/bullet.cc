#include "../include/bullet.h"

__USING_API

Bullet::Bullet(int x,
               int y,
               int rotation,
               Type type,
               sf::Texture *texture,
               float scale,
               int scene_offset_x,
               int scene_offset_y)
    : Entity(x, y, rotation, 32.0f, type, 1, scale, scene_offset_x, scene_offset_y) {
    this->set_graphics(texture);
}

void Bullet::update_behaviour() {
    if (this->can_move()) {
        int x = 0;
        int y = 0;

        switch (this->rotation) {
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
