#include "../include/spaceship.h"

__USING_API

Spaceship::Spaceship(int x, int y, float rotation, float speed, Type type, int health)
    : Entity(x, y, rotation, speed, type) {
    this->_health = health;
    this->_shooting = false;
    this->_sem = new Semaphore(1);
}

Spaceship::~Spaceship() {
    if (this->_sem) {
        delete this->_sem;
        this->_sem = nullptr;
    }
}

void Spaceship::kill() {
    this->_health = 0;
}

int Spaceship::get_shot_spawn_x() {
    int diff_x = 0;
    int half_width = static_cast<int>(this->get_shape()->get_width() * 0.5);
    int rotation = this->get_rotation();

    if (rotation == 90) {
        diff_x = half_width;
    } else if (rotation == 270) {
        diff_x = -half_width;
    }

    return this->get_position()[0] + diff_x;
}

int Spaceship::get_shot_spawn_y() {
    int diff_y = 0;
    int half_height = static_cast<int>(this->get_shape()->get_height() * 0.5);
    int rotation = this->get_rotation();

    if (rotation == 0) {
        diff_y = -half_height;
    } else if (rotation == 180) {
        diff_y = half_height;
    }

    return this->get_position()[1] + diff_y;
}
