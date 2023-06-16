#ifndef player_h
#define player_h

#include "../os/include/traits.h"
#include "../os/include/semaphore.h"
#include "spaceship.h"
// #include "game.h"

__USING_API

class Player : public Spaceship {
   public:
    Player() {}
    Player(int x, int y, sf::Texture *texture, sf::Color color, float scale, float speed);
    ~Player();
    static void update_behaviour(Player *player);

   private:
   //  Game::Event _current_event;  // TODO: Fix circular dependency
    // TODO: Deveria ser alocaod no heap?
    Semaphore _event_sem;
};

#endif
