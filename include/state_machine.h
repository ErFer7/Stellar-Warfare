#ifndef state_machine_h
#define state_machine_h

#include "../os/include/traits.h"

__USING_API

class StateMachine {
   public:
    enum State { MENU, INGAME, PAUSED, GAMEOVER, EXIT };
    enum Event { IDLE, FORWARD, BACKWARD, LEFT_TURN, RIGHT_TURN, SHOOT, PAUSE_TOGGLE, QUIT, RESTART, PLAYER_DEATH };
    StateMachine() {}
    ~StateMachine() {}
};

#endif
