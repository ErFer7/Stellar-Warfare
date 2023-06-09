#include "include/input.h"

__USING_API

Input::Input(Game *game) {
    this->_thread = new Thread(RunDetection, this);
    this->_game = game;
}

Input::~Input() { delete this->_thread; }

void Input::RunDetection(Input *input) {
    sf::RenderWindow *window = Game::get_window();

    while (window->isOpen()) {
        sf::Event event;

        while (window->pollEvent(event)) {
            switch (event.type) {
                case sf::Event::Closed:
                    window->close();
                    break;
                case sf::Event::KeyPressed:
                    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) {
                        std::cout << "Keyboard esquerda!" << std::endl;
                    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) {
                        std::cout << "Keyboard direita!" << std::endl;
                    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) {
                        std::cout << "Keyboard para baixo!" << std::endl;
                    } else if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) {
                        std::cout << "Keyboard para cima!" << std::endl;
                    } else
                        std::cout << "Keyboard pressed = " << event.key.code << std::endl;
                    break;
            }
        }
    }
}
