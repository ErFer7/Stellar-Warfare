#include "../include/image.h"

__USING_API

Image::Image(Alignment alignment, int x, int y, int window_width, int window_height, sf::Texture* texture,
             sf::Color color, float scale)
    : Widget(alignment, x, y, window_width, window_height) {
    this->_sprite = new sf::Sprite();
    this->_sprite->setTexture(*texture);
    this->_sprite->setColor(color);
    this->_sprite->setScale(scale, scale);
    this->_sprite->setPosition(this->position[0], this->position[1]);
}

Image::~Image() { delete this->_sprite; }

void Image::update(sf::Texture* texture) { this->_sprite->setTexture(*texture); }

void Image::render(sf::RenderWindow* window) { window->draw(*this->_sprite); }
