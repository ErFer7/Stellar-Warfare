#include "../include/game.h"

#include <SFML/Graphics.hpp>

#include "../include/input.h"
#include "../include/renderer.h"
#include "../include/scene.h"
#include "../include/user_interface.h"
#include "../os/include/traits.h"

__USING_API

Input* Game::_input;
Scene* Game::_scene;
UserInterface* Game::_user_interface;
Renderer* Game::_renderer;
sf::RenderWindow* Game::_window;

void Game::init() {
    _window = new sf::RenderWindow(sf::VideoMode(1032, 768), "Stellar Warfare");
    _window->setFramerateLimit(60);

    _input = new Input();
    _scene = new Scene();
    _renderer = new Renderer();
    _user_interface = new UserInterface();
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
    delete _user_interface;
    delete _renderer;
    delete _window;
}

void Game::handle_event(StateMachine::Event event) {
    _input->handle_event(event);
    _scene->handle_event(event);
    _user_interface->handle_event(event);
    _renderer->handle_event(event);
}
