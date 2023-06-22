#ifndef input_h
#define input_h

#include "../os/include/traits.h"
#include "../os/include/semaphore.h"
#include "game.h"
#include "state_machine.h"
#include "thread_container.h"

__USING_API

class Input final : public ThreadContainer {
   public:
    Input();
    ~Input();

    void handle_event(StateMachine::Event event);

   private:
    inline void lock_input() { this->_input_sem->p(); }
    inline void unlock_input() { this->_input_sem->v(); }
    inline StateMachine::State get_internal_state() { return this->_internal_state; }
    static void update_detection(Input *input);

   private:
    StateMachine::State _internal_state;
    Semaphore *_input_sem;
};

#endif
