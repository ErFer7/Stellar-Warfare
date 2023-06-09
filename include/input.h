#ifndef input_h
#define input_h

#include "../os/include/traits.h"
#include "../os/include/thread.h"
#include "include/game.h"

__USING_API

class Input {
   public:
    Input(Game *game);
    ~Input();

   private:
    static void RunDetection(Input *input);

   private:
    Thread *_thread;
    Game *_game;

};

#endif
