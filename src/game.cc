#include "include/game.h"

__USING_API

State _state;
Input _input;
Scene _scene;
UserInterface _userInterface;
Renderer _renderer;

void Game::run() {
    // Inicializa o jogo
    _state = State::MENU;

    // Roda o jogo todo

    // Finaliza o jogo
}

State Game::get_state() { return this->_state; }
