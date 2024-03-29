#include "../include/entity.h"

#include <iostream>

__USING_API

unsigned int Entity::_id_counter = 0;

Entity::Entity(int x,
               int y,
               int rotation,
               float speed,
               Type type,
               int size,
               float scale,
               int scene_offset_x,
               int scene_offset_y) {
    this->_id = _id_counter++;
    this->_index = -1;
    this->_time_accumulator = 0;
    this->_target_move[0] = 0;
    this->_target_move[1] = 0;
    this->position[0] = x;
    this->position[1] = y;
    this->_scene_offset[0] = scene_offset_x;
    this->_scene_offset[1] = scene_offset_y;
    this->rotation = rotation;
    this->size = size;
    this->_speed = speed;
    this->_type = type;
    this->_scale = scale;
    this->_sprite = new sf::Sprite();
    this->_color = sf::Color(136, 192, 112, 255);
    this->_clock = new sf::Clock();
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

void Entity::set_target_move(int x, int y) {
    this->_target_move[0] = x;
    this->_target_move[1] = y;
    this->_has_target_move = true;
    this->_time_accumulator = 0;
}

void Entity::set_position_and_rotation(int x, int y, int rotation) {
    this->position[0] = x;
    this->position[1] = y;
    this->rotation = rotation;

    update_sprite();
}

void Entity::reset_target_move() {
    this->_target_move[0] = 0;
    this->_target_move[1] = 0;
    this->_has_target_move = false;
}

void Entity::update_sprite() {
    float pixel_x = (this->_sprite->getLocalBounds().width * this->_scale) / this->size;
    float pixel_y = (this->_sprite->getLocalBounds().height * this->_scale) / this->size;

    int x = pixel_x * (this->position[0] + 0.5f) + this->_scene_offset[0];
    int y = pixel_y * (this->position[1] + 0.5f) + this->_scene_offset[1];

    this->_sprite->setPosition(x, y);
    this->_sprite->setRotation(this->rotation);
}
