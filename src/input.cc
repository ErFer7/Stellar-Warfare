#include "../include/input.h"

#include <iostream>

__USING_API

void Input::init() { this->thread = new Thread(update_detection, this); }

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
                    Game::handle_event(StateMachine::Event::QUIT);
                    break;
                case sf::Event::KeyPressed:
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                        Game::handle_event(StateMachine::Event::LEFT_TURN);
                    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                        Game::handle_event(StateMachine::Event::RIGHT_TURN);
                    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                        Game::handle_event(StateMachine::Event::BACKWARD);
                    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                        Game::handle_event(StateMachine::Event::FORWARD);
                    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Space)) {
                        Game::handle_event(StateMachine::Event::SHOOT);
                    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::P)) {
                        Game::handle_event(StateMachine::Event::PAUSE_TOGGLE);
                    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::R)) {
                        Game::handle_event(StateMachine::Event::RESTART);
                    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Q)) {
                        Game::handle_event(StateMachine::Event::QUIT);
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
