#ifndef widget_h
#define widget_h

#include <SFML/Graphics.hpp>

#include "../os/include/traits.h"

__USING_API

class Widget {
   public:
    enum Alignment { TOP_LEFT, TOP, TOP_RIGHT, LEFT, CENTER, RIGHT, BOTTOM_LEFT, BOTTOM, BOTTOM_RIGHT };

    Widget(Alignment alignment, int x, int y, int window_width, int window_height);
    ~Widget() {}
    virtual void render(sf::RenderWindow* window) {}

   protected:
    int position[2];
};

#endif
