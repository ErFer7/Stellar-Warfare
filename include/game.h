#ifndef game_h
#define game_h

#include <SFML/Graphics.hpp>

#include "../os/include/traits.h"
#include "include/input.h"
#include "include/renderer.h"
#include "include/scene.h"
#include "include/state.h"
#include "include/user_interface.h"

__USING_API

class Game {
   public:
    Game() {}
    ~Game() {}
    static void init();
    static void run();
    static void free();
    State get_state();
    static sf::RenderWindow *get_window();

   private:
    // Será que deve ser tudo estático?
    static State _state;
    static Input _input;
    static Scene _scene;
    static UserInterface _user_interface;
    static Renderer _renderer;
    static sf::RenderWindow _window;
};

#endif
