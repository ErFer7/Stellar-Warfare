#include "../include/entity.h"
__USING_API

Entity::Entity(int x, int y, float rotation, float speed, int scale) {
    this->_position[0] = x * scale + scale / 2;
    this->_position[1] = y * scale + scale / 2;
    this->_rotation = rotation;
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

void Entity::render(sf::RenderWindow *window) {
    this->_sprite.setPosition(this->_position[0], this->_position[1]);  // TODO: Alinhar à grade

    window->draw(this->_sprite);
}

void Entity::set_graphics(sf::Texture *texture, sf::Color color) {
    this->_sprite.setTexture(*texture);
    this->_sprite.setColor(color);
    this->_sprite.setScale(this->_scale, this->_scale);
}

void Entity::set_shape(int width, int height, Type *type) {
    if (this->_shape) {
        delete this->_shape;
    }

    this->_shape = new Matrix<Type>(width, height, VOID);
    this->_shape->fill(width, height, type);
}
