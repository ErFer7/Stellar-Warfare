#ifndef input_h
#define input_h

#include "../os/include/traits.h"
#include "game.h"
#include "state_machine.h"
#include "thread_container.h"

__USING_API

class Input final : public ThreadContainer {
   public:
    Input();
    ~Input() {}

   private:
    static void update_detection(Input *input);
};

#endif
