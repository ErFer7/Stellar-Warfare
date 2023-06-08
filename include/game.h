#ifndef game
#define game

#include "../os/include/traits.h"
#include "include/state.h"

__USING_API

class Game {
   public:
    Game();
    ~Game();
    void run();
    State get_state();

   private:
    State _state;
};

#endif
