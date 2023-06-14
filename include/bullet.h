#ifndef bullet_h
#define bullet_h

#include "../os/include/traits.h"
#include "entity.h"

__USING_API

class Bullet : public Entity {
   public:
    Bullet() {}
    Bullet(int x, int y, sf::Texture *texture);
    ~Bullet();

   private:
};

#endif