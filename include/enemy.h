#ifndef enemy_h
#define enemy_h

#include "../os/include/traits.h"
#include "spaceship.h"

__USING_API

class Enemy : public Spaceship {
   public:
    Enemy() {}
    Enemy(int x,
          int y,
          int rotation,
          float speed,
          sf::Texture* texture,
          float scale,
          int scene_offset_x,
          int scene_offset_y);
    ~Enemy();
    static void update_behaviour(Enemy* enemy);
    void tell_result(int player_x, int player_y, bool successfully_moved);

   private:
    inline int get_algorithm() { return this->_algorithm; }
    inline int* get_last_player_position() { return this->_last_player_position; }
    inline bool successfully_moved() { return this->_successfully_moved; }
    void set_last_move(int x, int y);
    inline int* get_last_move() { return this->_last_move; }
    void random_alg();
    void spinner_alg();
    void walker_alg();
    void hunter_alg();
    void shooter_alg();

   private:
    int _algorithm;
    int _last_player_position[2];
    int _last_move[2];
    bool _successfully_moved;
};

#endif
