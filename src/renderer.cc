#include "../include/renderer.h"

__USING_API

Renderer::Renderer(sf::Color background_color) {
    this->_background_color = background_color;
}

void Renderer::init() { this->thread = new Thread(render, this); }

void Renderer::render(Renderer *renderer) {
    sf::RenderWindow *window = Game::get_window();

    while (true) {
        Game::lock_state();
        if (Game::get_state() == StateMachine::State::EXIT) {
            Game::unlock_state();
            break;
        }
        Game::unlock_state();

        window->clear(renderer->get_background_color());

        Game::get_scene()->render(window);

        window->display();
        Thread::yield();
    }

    renderer->get_thread()->thread_exit(0);
}
