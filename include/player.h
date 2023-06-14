#ifndef player_h
#define player_h

#include "../os/include/traits.h"
#include "spaceship.h"

__USING_API

class Player : public Spaceship {
   public:
    Player() {}
    Player(int x, int y, sf::Texture *texture);
    ~Player();

   private:
};

#endif
