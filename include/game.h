#ifndef game_h
#define game_h

#include <SFML/Graphics.hpp>

#include "../os/include/semaphore.h"
#include "../os/include/traits.h"
#include "input.h"
#include "renderer.h"
#include "scene.h"
#include "state_machine.h"
#include "user_interface.h"

__USING_API

class Input;
class Scene;
class Renderer;

class Game {
   public:
    Game() {}
    ~Game() {}
    static void init();
    static void run();
    static void free();
    static void handle_event(StateMachine::Event event);
    static inline StateMachine::State get_state() { return _state; }
    static inline sf::RenderWindow *get_window() { return _window; }
    static inline Scene *get_scene() { return _scene; }
    static inline void lock_state() { _state_sem->p(); }
    static inline void unlock_state() { _state_sem->v(); }

   private:
    static StateMachine::State _state;  // Compartilhado com o Input (W e R), Scene (R) e Renderer (R) [LOCKED]
    static sf::RenderWindow *_window;
    static Input *_input;
    static Scene *_scene;
    static Renderer *_renderer;
    static UserInterface *_user_interface;
    static Semaphore *_state_sem;
};

#endif
