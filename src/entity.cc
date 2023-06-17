#include "../include/entity.h"

#include <math.h>
#include <iostream>

__USING_API

Entity::Entity(int x, int y, int rotation, float speed, int scale) {
    this->_position[0] = x;
    this->_position[1] = y;
    this->_rotation = rotation;
    this->_target_position[0] = this->_position[0];
    this->_target_position[1] = this->_position[1];
    this->_target_rotation = this->_rotation;
    this->_has_target_movement = false;
    this->_shape = nullptr;
    this->_speed = speed;
    this->_scale = scale;
}

Entity::~Entity() {
    if (this->_shape) {
        delete this->_shape;
        this->_shape = nullptr;
    }
}

void Entity::render(sf::RenderWindow *window) { window->draw(this->_sprite); }

void Entity::set_graphics(sf::Texture *texture, sf::Color color) {
    this->_sprite.setTexture(*texture);
    this->_sprite.setColor(color);
    this->_sprite.setScale(this->_scale, this->_scale);
    this->_sprite.setOrigin(this->_sprite.getLocalBounds().width * 0.5, this->_sprite.getLocalBounds().height * 0.5);
    update_sprite();
}

void Entity::set_shape(int width, int height, Type *type) {
    if (this->_shape) {
        delete this->_shape;
    }

    this->_shape = new Matrix<Type>(width, height, VOID);
    this->_shape->fill(width, height, type);
}

void Entity::move(int direction, int rotation) {
    this->_rotation += rotation;
    this->_rotation %= 360;
    this->_position[0] += (int)sin(this->_rotation * M_PI / 180) * direction;
    this->_position[1] += (int)cos(this->_rotation * M_PI / 180) * -direction;
    update_sprite();
}

void Entity::update_sprite() {
    int x = this->_position[0] * this->_scale + this->_scale * 0.5;
    int y = this->_position[1] * this->_scale + this->_scale * 0.5;

    this->_sprite.setPosition(x, y);
    this->_sprite.setRotation(this->_rotation);
}

void Entity::set_target_move(int direction, int rotation) {
    this->_target_rotation += rotation;
    this->_target_rotation %= 360;
    this->_target_position[0] += (int)sin(this->_target_rotation * M_PI / 180) * direction;
    this->_target_position[1] += (int)cos(this->_target_rotation * M_PI / 180) * -direction;
    this->_has_target_movement = true;
}
