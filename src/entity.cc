#include "../include/entity.h"

__USING_API

Entity::Entity(int x, int y, float rotation, sf::Texture *texture) {
    this->_position[0] = x;
    this->_position[1] = y;
    this->_rotation = rotation;
    this->_sprite.setTexture(*texture);
    this->_sprite.setColor(sf::Color(15, 45, 15, 255));
    this->_sprite.setScale(24.0f, 24.0f);
}

Entity::~Entity() {}

void Entity::render(sf::RenderWindow *window) {
    this->_sprite.setPosition(this->_position[0], this->_position[1]);  // TODO: Alinhar à grade
    window->draw(this->_sprite);
}
