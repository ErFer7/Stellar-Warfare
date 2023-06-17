#ifndef player_h
#define player_h

#include "../os/include/traits.h"
#include "../os/include/semaphore.h"
#include "spaceship.h"
#include "state_machine.h"

__USING_API

class Player final : public Spaceship {
   public:
    Player() { this->_event_sem = nullptr; }
    Player(int x, int y, sf::Texture *texture, sf::Color color, float scale);
    ~Player();
    void init() final;
    static void update_behaviour(Player *player);
    void set_control_event(StateMachine::Event event);

   private:
    StateMachine::Event _current_event;
    Semaphore *_event_sem;
};

#endif
