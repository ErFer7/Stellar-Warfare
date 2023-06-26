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
    Spaceship() {
        this->_sem = nullptr;
        this->_firerate_clock = nullptr;
    }
    Spaceship(int x,
              int y,
              float rotation,
              float speed,
              Type type,
              int health,
              float firerate,
              float scale,
              int scene_offset_x,
              int scene_offset_y);
    ~Spaceship();

    inline int get_health() { return this->_health; }
    inline bool is_shooting() { return this->_shooting; }
    inline void lock() { this->_sem->p(); }
    inline void unlock() { this->_sem->v(); }
    inline void reset_shooting() { this->_shooting = false; }
    inline void apply_damage(int damage) { this->_health -= damage; }
    int get_shot_spawn_x();
    int get_shot_spawn_y();
    bool can_shoot();
    void kill();
    void shoot();

   private:
    int _health;
    float _firerate;  // Taxa de tiros por segundo
    float _shot_time_accumulator;
    bool _shooting;
    Semaphore *_sem;
    sf::Clock *_firerate_clock;
};

#endif