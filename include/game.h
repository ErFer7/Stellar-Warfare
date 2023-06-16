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
class Renderer;

class Game {
   public:
    enum State { MENU, INGAME, PAUSED, GAMEOVER };
    enum Event { FORWARD, BACKWARD, LEFT_TURN, RIGHT_TURN, SHOOT, PAUSE, QUIT, RESTART, PLAYER_DEATH };

    Game() {}
    ~Game() {}
    static void init();
    static void run();
    static void free();
    static void send_event(Event event);
    static inline State get_state() { return _state; }
    static inline sf::RenderWindow *get_window() { return &_window; }
    static inline Scene *get_scene() { return &_scene; }

   private:
    // TODO: Será que deve ser tudo estático?
    // TODO: Deveria ser alocado no heap ou na stack?
    static sf::RenderWindow _window;
    static State _state;
    static Input _input;
    static Scene _scene;
    static Renderer _renderer;
    static UserInterface _user_interface;
};

#endif
