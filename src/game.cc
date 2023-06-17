#include "../include/game.h"

#include <SFML/Graphics.hpp>

#include "../include/input.h"
#include "../include/renderer.h"
#include "../include/scene.h"
#include "../include/user_interface.h"
#include "../os/include/traits.h"

__USING_API

StateMachine::State Game::_state;
Input *Game::_input;
Scene *Game::_scene;
UserInterface *Game::_user_interface;
Renderer *Game::_renderer;
sf::RenderWindow *Game::_window;
Semaphore *Game::_state_sem;

void Game::init() {
    _state = StateMachine::State::MENU;
    _window = new sf::RenderWindow(sf::VideoMode(1024, 768), "INE5412 Game");  // TODO: Conferir se a resolução é boa
    _window->setKeyRepeatEnabled(false);
    _window->setFramerateLimit(60);
    _state_sem = new Semaphore(1);

    _input = new Input();
    _scene = new Scene(sf::Color(15, 45, 15, 255), 24.0f, 10, 10);
    _renderer = new Renderer(sf::Color(155, 188, 15, 255));

    _input->init();
    _scene->init();
    _renderer->init();
}

void Game::run() {
    init();
    free();
}

void Game::free() {
    _input->join();
    _scene->join();
    _renderer->join();
    _window->close();

    delete _state_sem;
    delete _input;
    delete _scene;
    delete _renderer;
    delete _window;
}

void Game::handle_event(StateMachine::Event event) {
    switch (event) {
        case StateMachine::Event::PAUSE_TOGGLE:
            set_state(_state == StateMachine::State::PAUSED ? StateMachine::State::INGAME
                                                            : StateMachine::State::PAUSED);
            break;
        case StateMachine::Event::RESTART:
            set_state(StateMachine::State::INGAME);
            break;
        case StateMachine::Event::PLAYER_DEATH:
            set_state(StateMachine::State::GAMEOVER);
            break;
        case StateMachine::Event::QUIT:
            set_state(StateMachine::State::EXIT);
            break;
        default:
            _scene->handle_player_control_event(event);
            break;
    }
}

void Game::set_state(StateMachine::State state) {
    lock_state();
    _state = state;
    unlock_state();
}
