#ifndef renderer_h
#define renderer_h

#include <SFML/Graphics.hpp>

#include "../os/include/semaphore.h"
#include "../os/include/traits.h"
#include "game.h"
#include "state_machine.h"
#include "thread_container.h"

__USING_API

class Renderer final : public ThreadContainer {
   public:
    Renderer();
    ~Renderer();

    inline sf::Color get_background_color() { return this->_background_color; }
    void handle_event(StateMachine::Event event);

   private:
    inline void lock_renderer() { this->_renderer_sem->p(); }
    inline void unlock_renderer() { this->_renderer_sem->v(); }
    inline StateMachine::State get_internal_state() { return this->_internal_state; }
    static void render(Renderer *renderer);

   private:
    sf::Color _background_color;
    StateMachine::State _internal_state;
    Semaphore *_renderer_sem;
};

#endif