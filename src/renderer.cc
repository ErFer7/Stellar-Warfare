#include "../include/renderer.h"

__USING_API

Renderer::Renderer() { this->_thread = nullptr; }

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
        window->clear();
        window->display();
        Thread::yield();
    }

    renderer->get_thread()->thread_exit(0);
}
