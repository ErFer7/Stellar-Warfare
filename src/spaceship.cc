#include "../include/spaceship.h"

__USING_API

Spaceship::Spaceship(int x, int y, float rotation, float speed, Type type, int health, float firerate, float scale, int scene_offset_x, int scene_offset_y)
    : Entity(x, y, rotation, speed, type, 3, scale, scene_offset_x, scene_offset_y) {
    this->_shooting = false;
    this->_shot_time_accumulator = 0.0f;
    this->_health = health;
    this->_firerate = firerate;
    this->_sem = new Semaphore(1);
    this->_firerate_clock = new sf::Clock();
}

Spaceship::~Spaceship() {
    delete this->_sem;
    delete this->_firerate_clock;
}

void Spaceship::kill() { this->_health = 0; }

int Spaceship::get_shot_spawn_x() {
    int diff_x = 0;
    int half_width = this->get_size() * 0.5;
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
    int half_height = this->get_size() * 0.5;
    int rotation = this->get_rotation();

    if (rotation == 0) {
        diff_y = -half_height;
    } else if (rotation == 180) {
        diff_y = half_height;
    }

    return this->get_position()[1] + diff_y;
}

bool Spaceship::can_shoot() {
    this->_shot_time_accumulator += this->_firerate_clock->getElapsedTime().asSeconds();
    this->_firerate_clock->restart();

    return this->_shot_time_accumulator >= 1.0f / this->_firerate;
}

void Spaceship::shoot() {
    this->_shooting = true;
    this->_shot_time_accumulator = 0.0f;
}