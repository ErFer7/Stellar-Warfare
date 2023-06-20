#include "../include/entity.h"

#include <iostream>

__USING_API

unsigned int Entity::_id_counter = 0;

Entity::Entity(int x, int y, int rotation, float speed, Type type, int size) {
    this->_id = _id_counter++;
    this->_index = -1;
    this->_position[0] = x;
    this->_position[1] = y;
    this->_rotation = rotation;
    this->_target_direction = 0;
    this->_target_rotation = 0;
    this->_sprite = new sf::Sprite();
    this->_size = size;
    this->_speed = speed;
    this->_time_accumulator = 0;
    this->_scale = 24;
    this->_color = sf::Color(15, 45, 15, 255);
    this->_type = type;
    this->_clock = new sf::Clock();
    this->_already_simulated = false;
}

Entity::~Entity() {
    if (this->_sprite) {
        delete this->_sprite;
        this->_sprite = nullptr;
    }

    if (this->_clock) {
        delete this->_clock;
        this->_clock = nullptr;
    }
}

void Entity::render(sf::RenderWindow *window) { window->draw(*this->_sprite); }

void Entity::set_graphics(sf::Texture *texture) {
    this->_sprite->setTexture(*texture);
    this->_sprite->setColor(this->_color);
    this->_sprite->setScale(this->_scale, this->_scale);
    this->_sprite->setOrigin(this->_sprite->getLocalBounds().width * 0.5, this->_sprite->getLocalBounds().height * 0.5);
    update_sprite();
}

bool Entity::can_move() {
    this->_time_accumulator += this->_clock->getElapsedTime().asSeconds();
    this->_clock->restart();

    return this->_time_accumulator >= 1.0f / this->_speed;
}

void Entity::set_target_move(int direction, int rotation) {
    this->_target_direction = direction;
    this->_target_rotation = rotation;
    this->_has_target_move = true;
    this->_time_accumulator = 0;
}

void Entity::set_position_and_rotation(int x, int y, int rotation) {
    this->_position[0] = x;
    this->_position[1] = y;
    this->_rotation = rotation;

    update_sprite();
}

void Entity::reset_target_move() {
    this->_target_direction = 0;
    this->_target_rotation = 0;
    this->_has_target_move = false;
}

void Entity::update_sprite() {
    int x = this->_position[0] * this->_scale + this->_scale * 0.5;
    int y = this->_position[1] * this->_scale + this->_scale * 0.5;

    this->_sprite->setPosition(x, y);
    this->_sprite->setRotation(this->_rotation);
}
