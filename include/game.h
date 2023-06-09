#ifndef game_h
#define game_h

#include <SFML/Graphics.hpp>

#include "../os/include/traits.h"
#include "input.h"
#include "renderer.h"
#include "scene.h"
#include "user_interface.h"

__USING_API

class Input;
class Scene;

class Game {
   public:
    enum State { MENU, INGAME, PAUSED, GAMEOVER };
    enum Event { UP, DOWN, LEFT, RIGHT, SHOOT, PAUSE, QUIT, RESTART, PLAYER_DEATH };

    Game() {}
    ~Game() {}
    static void init();
    static void run();
    static void free();
    static void send_event(Event event);
    static State get_state();
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
