#ifndef enemy_h
#define enemy_h

#include "../os/include/traits.h"
#include "spaceship.h"

__USING_API

class Enemy : public Spaceship {
   public:
    Enemy() {}
    Enemy(int x, int y, int rotation, float speed, sf::Texture *texture, float scale, int scene_offset_x,
          int scene_offset_y);
    ~Enemy();
    static void update_behaviour(Enemy *enemy);

   private:
    inline int get_algorithm() { return this->_algorithm; }
    void random_alg();
    void spinner_alg();
    void walker_alg();
    void hunter_alg();
    void sniper_alg();
    void scared_alg();

   private:
    int _algorithm;
};

#endif
