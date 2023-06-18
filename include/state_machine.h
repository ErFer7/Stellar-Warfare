#ifndef state_machine_h
#define state_machine_h

#include "../os/include/traits.h"

__USING_API

class StateMachine {
   public:
    enum State { NONINITIALIZED, INGAME, PAUSED, GAMEOVER, EXIT };
    enum Event { IDLE, UP, DOWN, LEFT, RIGHT, SPACE, P_KEY, Q_KEY, R_KEY, PLAYER_DEATH };
    StateMachine() {}
    ~StateMachine() {}
};

#endif
