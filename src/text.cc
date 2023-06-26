#include "../include/text.h"

__USING_API

Text::Text(Alignment alignment,
           int x,
           int y,
           int window_width,
           int window_height,
           sf::Font *font,
           sf::Color color,
           sf::Color shadow_color,
           int size,
           sf::String text)
    : Widget(alignment, x, y, window_width, window_height) {
    this->_text = new sf::Text();
    this->_shadow = new sf::Text();

    this->_text->setFont(*font);
    this->_text->setString(text);
    this->_text->setFillColor(color);
    this->_text->setCharacterSize(size);
    this->_text->setPosition(this->position[0], this->position[1]);

    this->_shadow->setFont(*font);
    this->_shadow->setString(text);
    this->_shadow->setFillColor(shadow_color);
    this->_shadow->setCharacterSize(size);
    this->_shadow->setPosition(this->position[0], this->position[1] + size / 6);
}

Text::~Text() {
    delete this->_text;
    delete this->_shadow;
}

void Text::update(sf::String text) {
    this->_text->setString(text);
    this->_shadow->setString(text);
}

void Text::render(sf::RenderWindow *window) {
    window->draw(*this->_shadow);
    window->draw(*this->_text);
}
