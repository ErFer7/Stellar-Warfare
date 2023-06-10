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
    inline Thread *get_thread() { return this->_thread; }

   private:
    static void update_detection(Input *input);

   private:
    Thread *_thread;
};

#endif
