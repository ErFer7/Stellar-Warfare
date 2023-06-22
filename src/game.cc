#include "../include/game.h"

#include <SFML/Graphics.hpp>

#include "../include/input.h"
#include "../include/renderer.h"
#include "../include/scene.h"
#include "../include/user_interface.h"
#include "../os/include/traits.h"

__USING_API

Input *Game::_input;
Scene *Game::_scene;
UserInterface *Game::_user_interface;
Renderer *Game::_renderer;
sf::RenderWindow *Game::_window;

void Game::init() {
    _window = new sf::RenderWindow(sf::VideoMode(1024, 768), "INE5412 Game");  // TODO: Conferir se a resolução é boa
    _window->setKeyRepeatEnabled(true);                                        // TODO: Conferir se o key repeat é bom
    _window->setFramerateLimit(60);                                            // TODO: Conferir se o framerate é bom

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

    delete _input;
    delete _scene;
    delete _renderer;
    delete _window;
}

void Game::handle_event(StateMachine::Event event) {
    _input->handle_event(event);
    _scene->handle_event(event);
    _renderer->handle_event(event);
}
