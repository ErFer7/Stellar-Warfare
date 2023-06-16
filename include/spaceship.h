#ifndef spaceship_h
#define spaceship_h

#include "../os/include/thread.h"
#include "../os/include/traits.h"
#include "entity.h"

__USING_API

class Spaceship : public Entity {
   public:
    Spaceship() { this->thread = nullptr; }
    Spaceship(int x, int y, float rotation, sf::Texture *texture, sf::Color color, float scale, float speed);
    ~Spaceship();
    inline int get_health() { return this->_health; }
    inline Thread *get_thread() { return this->thread; }

   protected:
    Thread *thread;

   private:
    int _health;
};

#endif