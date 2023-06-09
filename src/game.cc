#include "include/game.h"

#include "game.h"

__USING_API

State _state;
Input _input;
Scene _scene;
UserInterface _userInterface;
Renderer _renderer;

void Game::init() {
    _state = State::MENU;
    new (&_window) sf::RenderWindow(sf::VideoMode(800, 600), "SFML works!");

    _window.setKeyRepeatEnabled(false);  // TODO: Ver se é necessário
}

void Game::run() {
    init();

    // Roda

    free();
}

void Game::free() {
    // Finaliza o jogo
}

State Game::get_state() { return this->_state; }

sf::RenderWindow *Game::get_window() { return &_window; }
