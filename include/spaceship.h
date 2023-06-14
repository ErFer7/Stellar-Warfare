#ifndef spaceship_h
#define spaceship_h

#include "../os/include/thread.h"
#include "../os/include/traits.h"
#include "entity.h"

__USING_API

class Spaceship : public Entity {
   public:
    Spaceship() {}
    Spaceship(int x, int y, float rotation, sf::Texture *texture);
    ~Spaceship();

   private:
};

#endif