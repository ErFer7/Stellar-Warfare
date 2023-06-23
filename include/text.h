#ifndef text_h
#define text_h

#include <SFML/Graphics.hpp>

#include "../os/include/traits.h"
#include "widget.h"

__USING_API

class Text final : public Widget {
   public:
    Text(Alignment alignment, int x, int y, int window_width, int window_height, sf::Font* font, sf::Color color,
         sf::Color shadow_color, int size, sf::String text);
    ~Text();
    void update(sf::String text);
    void render(sf::RenderWindow* window) override;

   private:
    sf::Text* _text;
    sf::Text* _shadow;
};

#endif
