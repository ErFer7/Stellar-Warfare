#ifndef renderer_h
#define renderer_h

#include "../os/include/traits.h"
#include "game.h"
#include "thread_container.h"

__USING_API

class Renderer final : public ThreadContainer {
   public:
    Renderer(sf::Color background_color);
    ~Renderer() {}
    void init() final;
    inline sf::Color get_background_color() { return this->_background_color; }

   private:
    static void render(Renderer *renderer);

   private:
    sf::Color _background_color;
};

#endif