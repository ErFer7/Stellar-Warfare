#include "../include/input.h"

#include <iostream>

__USING_API

Input::Input() { this->thread = new Thread(update_detection, this); }

void Input::update_detection(Input *input) {
    sf::RenderWindow *window = Game::get_window();

    while (true) {
        Game::lock_state();
        if (Game::get_state() == StateMachine::State::EXIT) {
            Game::unlock_state();
            break;
        }
        Game::unlock_state();

        sf::Event event;

        while (window->pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    Game::handle_event(StateMachine::Event::Q_KEY);
                    break;
                case sf::Event::KeyPressed:
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                        Game::handle_event(StateMachine::Event::LEFT);
                    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                        Game::handle_event(StateMachine::Event::RIGHT);
                    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                        Game::handle_event(StateMachine::Event::DOWN);
                    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                        Game::handle_event(StateMachine::Event::UP);
                    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
                        Game::handle_event(StateMachine::Event::SPACE);
                    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::P)) {
                        Game::handle_event(StateMachine::Event::P_KEY);
                    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
                        Game::handle_event(StateMachine::Event::R_KEY);
                    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
                        Game::handle_event(StateMachine::Event::Q_KEY);
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
