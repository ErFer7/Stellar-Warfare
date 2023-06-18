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
    _state = StateMachine::State::NONINITIALIZED;
    _window = new sf::RenderWindow(sf::VideoMode(1024, 768), "INE5412 Game");  // TODO: Conferir se a resolução é boa
    _window->setKeyRepeatEnabled(false);
    _window->setFramerateLimit(60);
    _state_sem = new Semaphore(1);

    _input = new Input();
    _scene = new Scene();
    _renderer = new Renderer();
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
        case StateMachine::Event::P_KEY:
            lock_state();
            if (_state == StateMachine::State::NONINITIALIZED) {
                _state = StateMachine::State::INGAME;
                _scene->start_game();
            } else if (_state != StateMachine::State::GAMEOVER) {
                if (_state == StateMachine::State::PAUSED) {
                    _state = StateMachine::State::INGAME;
                } else {
                    _state = StateMachine::State::PAUSED;
                }
            }
            unlock_state();
            break;
        case StateMachine::Event::R_KEY:
            lock_state();
            _state = StateMachine::State::INGAME;
            unlock_state();
            break;
        case StateMachine::Event::PLAYER_DEATH:
            lock_state();
            _state = StateMachine::State::GAMEOVER;
            unlock_state();
            break;
        case StateMachine::Event::Q_KEY:
            lock_state();
            _state = StateMachine::State::EXIT;
            unlock_state();
            break;
        default:
            _scene->handle_player_control_event(event);
            break;
    }
}
