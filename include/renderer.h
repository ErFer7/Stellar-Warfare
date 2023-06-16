#ifndef renderer_h
#define renderer_h

#include "../os/include/thread.h"
#include "../os/include/traits.h"
#include "game.h"

__USING_API

class Renderer : public Thread {
   public:
    Renderer();
    ~Renderer();
    void init();
    void stop();
    inline Thread *get_thread() { return this->_thread; }
    inline sf::Color get_background_color() { return this->_background_color; }

   private:
    static void render(Renderer *renderer);

   private:
    Thread *_thread;
    sf::Color _background_color;
};

#endif