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
    static inline sf::RenderWindow *get_window() { return _window; }
    static inline Scene *get_scene() { return _scene; }

   private:
    static sf::RenderWindow *_window;
    static Input *_input;
    static Scene *_scene;
    static Renderer *_renderer;
    static UserInterface *_user_interface;
};

#endif
