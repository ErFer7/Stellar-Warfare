#include "../include/user_interface.h"

__USING_API

UserInterface::UserInterface() {
    int window_width = 1032;
    int window_height = 768;
    sf::Color default_color = sf::Color(136, 192, 112, 255);
    sf::Color default_shadow_color = sf::Color(52, 104, 86, 255);

    this->_internal_state = StateMachine::State::NONINITIALIZED;
    this->_user_interface_sem = new Semaphore(1);

    this->_border_texture = new sf::Texture();
    this->_empty_heart_texture = new sf::Texture();
    this->_full_heart_texture = new sf::Texture();
    this->_font = new sf::Font();

    if (!this->_border_texture->loadFromFile("assets/sprites/border.png")) {
        throw std::runtime_error("Could not load border texture");
    }

    if (!this->_empty_heart_texture->loadFromFile("assets/sprites/empty_heart.png")) {
        throw std::runtime_error("Could not load border texture");
    }

    if (!this->_full_heart_texture->loadFromFile("assets/sprites/full_heart.png")) {
        throw std::runtime_error("Could not load border texture");
    }

    if (!this->_font->loadFromFile("assets/fonts/joystix monospace.ttf")) {
        throw std::runtime_error("Could not load font");
    }

    this->_border =
        new Image(Image::TOP_LEFT, 0, 0, window_width, window_height, this->_border_texture, sf::Color::White, 4.0f);

    this->_title = new Text(Text::TOP_RIGHT, -232, 32, window_width, window_height, this->_font, default_color,
                            default_shadow_color, 32, "Stellar\nWarfare");

    this->_state_info = new Text(Text::TOP_RIGHT, -232, 128, window_width, window_height, this->_font, default_color,
                                 default_shadow_color, 16, "Noninitialized:\nPress P to play");

    this->_score_label = new Text(Text::RIGHT, -232, -164, window_width, window_height, this->_font, default_color,
                                  default_shadow_color, 16, "Score: ");

    this->_score = new Text(Text::RIGHT, -150, -164, window_width, window_height, this->_font, default_color,
                            default_shadow_color, 16, "0");

    this->_health_label = new Text(Text::RIGHT, -232, -128, window_width, window_height, this->_font, default_color,
                                   default_shadow_color, 16, "HP: ");

    this->_heart1 = new Image(Image::RIGHT, -180, -128, window_width, window_height, this->_full_heart_texture,
                              default_color, 2.0f);

    this->_heart2 = new Image(Image::RIGHT, -150, -128, window_width, window_height, this->_full_heart_texture,
                              default_color, 2.0f);

    this->_heart3 = new Image(Image::RIGHT, -120, -128, window_width, window_height, this->_full_heart_texture,
                              default_color, 2.0f);

    this->_level_label = new Text(Text::RIGHT, -232, -92, window_width, window_height, this->_font, default_color,
                                  default_shadow_color, 16, "Level: ");

    this->_level = new Text(Text::RIGHT, -150, -92, window_width, window_height, this->_font, default_color,
                            default_shadow_color, 16, "1");

    this->_info = new Text(Text::BOTTOM_RIGHT, -232, -128, window_width, window_height, this->_font, default_color,
                           default_shadow_color, 12, "This is definitely\na game");

    this->_authors = new Text(Text::BOTTOM_RIGHT, -232, -56, window_width, window_height, this->_font, default_color,
                              default_shadow_color, 12, "INE5412 - 2023.2\nEric - Victor");
}

UserInterface::~UserInterface() {
    delete this->_border_texture;
    delete this->_empty_heart_texture;
    delete this->_full_heart_texture;
    delete this->_font;
    delete this->_border;
    delete this->_title;
    delete this->_state_info;
    delete this->_score_label;
    delete this->_score;
    delete this->_health_label;
    delete this->_level_label;
    delete this->_level;
    delete this->_info;
    delete this->_authors;
    delete this->_heart1;
    delete this->_heart2;
    delete this->_heart3;
    delete this->_user_interface_sem;
}

void UserInterface::render(sf::RenderWindow* window) {
    this->_user_interface_sem->p();

    this->_border->render(window);
    this->_title->render(window);
    this->_state_info->render(window);
    this->_score_label->render(window);
    this->_score->render(window);
    this->_health_label->render(window);
    this->_heart1->render(window);
    this->_heart2->render(window);
    this->_heart3->render(window);
    this->_level_label->render(window);
    this->_level->render(window);
    this->_info->render(window);
    this->_authors->render(window);

    this->_user_interface_sem->v();
}

void UserInterface::handle_event(StateMachine::Event event) {
    this->_user_interface_sem->p();
    switch (event) {
        case StateMachine::Event::P_KEY:
            if (this->_internal_state == StateMachine::State::NONINITIALIZED) {
                this->_state_info->update("Ingame:\nPress P to\npause");
                this->_internal_state = StateMachine::State::INGAME;
            } else if (this->_internal_state != StateMachine::State::GAMEOVER) {
                if (this->_internal_state == StateMachine::State::PAUSED) {
                    this->_state_info->update("Ingame:\nPress P to\npause");
                    this->_internal_state = StateMachine::State::INGAME;
                } else {
                    this->_state_info->update("Paused:\nPress P to\nresume");
                    this->_internal_state = StateMachine::State::PAUSED;
                }
            }
            break;
        case StateMachine::Event::R_KEY:
            if (this->_internal_state == StateMachine::State::GAMEOVER) {
                this->_state_info->update("Ingame:\nPress P to\npause");
                this->_internal_state = StateMachine::State::INGAME;
            }
            break;
        case StateMachine::Event::PLAYER_DEATH:
            this->_state_info->update("Gameover:\nPress R to\nrestart");
            this->_internal_state = StateMachine::State::GAMEOVER;
            break;
        case StateMachine::Event::Q_KEY:
            this->_internal_state = StateMachine::State::EXIT;
            break;
        default:
            break;
    }

    this->_user_interface_sem->v();
}
void UserInterface::update_score(int score) {
    this->_user_interface_sem->p();
    this->_score->update(std::to_string(score));

    this->_user_interface_sem->v();
}

void UserInterface::update_health(int health) {
    this->_user_interface_sem->p();
    switch (health) {
        case 0:
            this->_heart1->update(this->_empty_heart_texture);
            this->_heart2->update(this->_empty_heart_texture);
            this->_heart3->update(this->_empty_heart_texture);
            break;
        case 1:
            this->_heart1->update(this->_full_heart_texture);
            this->_heart2->update(this->_empty_heart_texture);
            this->_heart3->update(this->_empty_heart_texture);
            break;
        case 2:
            this->_heart1->update(this->_full_heart_texture);
            this->_heart2->update(this->_full_heart_texture);
            this->_heart3->update(this->_empty_heart_texture);
            break;
        case 3:
            this->_heart1->update(this->_full_heart_texture);
            this->_heart2->update(this->_full_heart_texture);
            this->_heart3->update(this->_full_heart_texture);
            break;
        default:
            break;
    }

    this->_user_interface_sem->v();
}

void UserInterface::update_level(int speed) {
    this->_user_interface_sem->p();
    this->_level->update(std::to_string(speed));
    this->_user_interface_sem->v();
}
