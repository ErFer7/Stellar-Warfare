#ifndef input_h
#define input_h

#include "../os/include/traits.h"
#include "../os/include/thread.h"
#include "game.h"

__USING_API

class Input {
   public:
    Input();
    ~Input();
    void init();
    void stop();

   private:
    static void UpdateDetection(Input *input);

   private:
    Thread *_thread;
};

#endif
