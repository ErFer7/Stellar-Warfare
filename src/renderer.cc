#include "../include/renderer.h"

__USING_API

Renderer::Renderer() {
    this->_thread = nullptr;
    this->_background_color = sf::Color(155, 188, 15, 255);  // Talvez seja melhor passar isso como parâmetro
}

Renderer::~Renderer() {
    if (!this->_thread) {
        delete this->_thread;
    }
}

void Renderer::init() { this->_thread = new Thread(render, this); }

void Renderer::stop() { this->_thread->join(); }

void Renderer::render(Renderer *renderer) {
    sf::RenderWindow *window = Game::get_window();

    while (window->isOpen()) {
        window->clear(renderer->get_background_color());

        Game::get_scene()->render(window);

        window->display();
        Thread::yield();
    }

    renderer->get_thread()->thread_exit(0);
}
