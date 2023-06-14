#include "../include/input.h"

#include <iostream>

__USING_API

Input::Input() { this->_thread = nullptr; }

Input::~Input() {
    if (!this->_thread) {
        delete this->_thread;
    }
}

void Input::init() { this->_thread = new Thread(update_detection, this); }

void Input::stop() { this->_thread->join(); }

void Input::update_detection(Input *input) {
    sf::RenderWindow *window = Game::get_window();

    while (window->isOpen()) {
        sf::Event event;

        while (window->pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    Game::send_event(Game::Event::QUIT);
                    break;
                case sf::Event::KeyPressed:
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                        Game::send_event(Game::Event::LEFT);
                        std::cout << "Keyboard esquerda!" << std::endl;
                    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                        Game::send_event(Game::Event::RIGHT);
                        std::cout << "Keyboard direita!" << std::endl;
                    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                        Game::send_event(Game::Event::DOWN);
                        std::cout << "Keyboard para baixo!" << std::endl;
                    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                        Game::send_event(Game::Event::UP);
                        std::cout << "Keyboard para cima!" << std::endl;
                    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
                        Game::send_event(Game::Event::SHOOT);
                        std::cout << "Keyboard pressed = " << event.key.code << std::endl;
                    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::P)) {
                        std::cout << "Keyboard pressed = " << event.key.code << std::endl;
                        Game::send_event(Game::Event::PAUSE);
                    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
                        std::cout << "Keyboard pressed = " << event.key.code << std::endl;
                        Game::send_event(Game::Event::RESTART);
                    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
                        std::cout << "Keyboard pressed = " << event.key.code << std::endl;
                        Game::send_event(Game::Event::QUIT);
                    }
                    break;
                default:
                    break;
            }
        }

        Thread::yield();
    }

    input->get_thread()->thread_exit(0);
}
