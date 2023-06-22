#include "../include/renderer.h"

__USING_API

Renderer::Renderer() {
    this->_background_color = sf::Color(4, 12, 16, 255);
    this->_internal_state = StateMachine::State::NONINITIALIZED;
    this->_renderer_sem = new Semaphore(1);
    this->thread = new Thread(render, this);
}

Renderer::~Renderer() {
    delete this->_renderer_sem;
}

void Renderer::handle_event(StateMachine::Event event) {
    this->lock_renderer();
    if (event == StateMachine::Event::Q_KEY) {
        this->_internal_state = StateMachine::State::EXIT;
    }
    this->unlock_renderer();
}

void Renderer::render(Renderer *renderer) {
    sf::RenderWindow *window = Game::get_window();

    while (true) {
        renderer->lock_renderer();
        if (renderer->get_internal_state() == StateMachine::State::EXIT) {
            renderer->unlock_renderer();
            break;
        }
        renderer->unlock_renderer();

        window->clear(renderer->get_background_color());

        Game::get_scene()->render(window);
        // TODO: Renderizar UI

        window->display();
        Thread::yield();
    }

    renderer->get_thread()->thread_exit(0);
}
