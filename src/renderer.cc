#include "../include/renderer.h"

__USING_API

Renderer::Renderer() {
    this->_background_color = sf::Color(155, 188, 15, 255);
    this->thread = new Thread(render, this);
}

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
        // TODO: Renderizar UI

        window->display();
        Thread::yield();
    }

    renderer->get_thread()->thread_exit(0);
}
