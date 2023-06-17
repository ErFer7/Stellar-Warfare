#ifndef spaceship_h
#define spaceship_h

#include "../os/include/thread.h"
#include "../os/include/traits.h"
#include "../os/include/semaphore.h"
#include "entity.h"
#include "thread_container.h"

__USING_API

class Spaceship : public Entity, public ThreadContainer {
   public:
    Spaceship() { this->_target_move_sem = nullptr; }
    Spaceship(int x, int y, float rotation, sf::Texture *texture, sf::Color color, float scale, float speed);
    ~Spaceship();
    inline int get_health() { return this->_health; }
    inline void lock_target_move() { this->_target_move_sem->p(); }
    inline void unlock_target_move() { this->_target_move_sem->v(); }

   private:
    int _health;
    Semaphore *_target_move_sem;
};

#endif