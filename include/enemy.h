#ifndef enemy_h
#define enemy_h

#include "../os/include/traits.h"
#include "spaceship.h"

__USING_API

class Enemy : public Spaceship {
   public:
    Enemy() {}
    Enemy(int x, int y, float rotation, float speed, sf::Texture *texture);
    ~Enemy();
    static void update_behaviour(Enemy *enemy);
};

#endif
