#ifndef spaceship_h
#define spaceship_h

#include "../os/include/semaphore.h"
#include "../os/include/thread.h"
#include "../os/include/traits.h"
#include "entity.h"
#include "thread_container.h"

__USING_API

class Spaceship : public Entity, public ThreadContainer {
   public:
    Spaceship() { this->_action_sem = nullptr; }
    Spaceship(int x, int y, float rotation, float speed, Type type, int health);
    ~Spaceship();
    inline int get_health() { return this->_health; }
    inline void lock_action() { this->_action_sem->p(); }
    inline void unlock_action() { this->_action_sem->v(); }
    inline void shoot() { this->_shooting = true; }
    inline bool is_shooting() { return this->_shooting; }
    inline void reset_shooting() { this->_shooting = false; }
    int get_shot_spawn_x();
    int get_shot_spawn_y();

   private:
    int _health;
    bool _shooting;
    Semaphore *_action_sem;
};

#endif