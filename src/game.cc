#include "../include/game.h"

#include <SFML/Graphics.hpp>

#include "../include/input.h"
#include "../include/renderer.h"
#include "../include/scene.h"
#include "../include/user_interface.h"
#include "../os/include/traits.h"

__USING_API

Game::State Game::_state;
Input Game::_input;
Scene Game::_scene;
UserInterface Game::_user_interface;
Renderer Game::_renderer;
sf::RenderWindow Game::_window;

void Game::init() {
    _state = Game::State::MENU;
    new (&_window) sf::RenderWindow(sf::VideoMode(800, 600), "SFML works!");
    _window.setKeyRepeatEnabled(false);

    _input.init();
    _scene.init();
}

void Game::run() {
    init();

    // Roda

    free();
}

void Game::free() {
    _input.stop();
    _scene.stop();
    // _window.close();
}

void Game::send_event(Event event) {
    switch (event) {
        case Event::QUIT:
            _window.close();
            break;
        default:
            break;
    }
}

Game::State Game::get_state() { return _state; }

sf::RenderWindow *Game::get_window() { return &_window; }
