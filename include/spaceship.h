#ifndef spaceship_h
#define spaceship_h

#include <iostream>

#include "../os/include/semaphore.h"
#include "../os/include/thread.h"
#include "../os/include/traits.h"
#include "entity.h"
#include "thread_container.h"

__USING_API

class Spaceship : public Entity, public ThreadContainer {
   public:
    Spaceship() { this->_sem = nullptr; }
    Spaceship(int x, int y, float rotation, float speed, Type type, int health);
    ~Spaceship();
    inline int get_health() { return this->_health; }
    inline void lock() { this->_sem->p(); }
    inline void unlock() { this->_sem->v(); }
    inline void shoot() { this->_shooting = true; }
    inline bool is_shooting() { return this->_shooting; }
    inline void reset_shooting() { this->_shooting = false; }
    inline void apply_damage(int damage) { this->_health -= damage; }
    void kill();
    int get_shot_spawn_x();
    int get_shot_spawn_y();

   private:
    int _health;
    bool _shooting;
    Semaphore *_sem;
};

#endif