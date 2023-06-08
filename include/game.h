#ifndef game_h
#define game_h

#include "../os/include/traits.h"
#include "include/state.h"
#include "include/scene.h"
#include "include/user_interface.h"
#include "include/input.h"
#include "include/renderer.h"

__USING_API

class Game {
   public:
    Game() {}
    ~Game() {}
    static void run();
    State get_state();

   private:
    // Será que deve ser tudo estático?
    static State _state;
    static Input _input;
    static Scene _scene;
    static UserInterface _userInterface;
    static Renderer _renderer;
};

#endif
