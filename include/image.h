#ifndef image_h
#define image_h

#include <SFML/Graphics.hpp>

#include "../os/include/traits.h"
#include "widget.h"

__USING_API

class Image final : public Widget {
   public:
    Image(Alignment alignment,
          int x,
          int y,
          int window_width,
          int window_height,
          sf::Texture *texture,
          sf::Color color,
          float scale);
    ~Image();

    void update(sf::Texture *texture);
    void render(sf::RenderWindow *window) override;

   private:
    sf::Sprite *_sprite;
};

#endif
