#include "../include/player.h"

#include <iostream>

__USING_API

Player::Player(int x, int y, sf::Texture *texture, sf::Color color, float scale)
    : Spaceship(x, y, 0.0f, texture, color, scale, 0.0f) {
    Type shape[] = {VOID, PLAYER, VOID, PLAYER, PLAYER, PLAYER, PLAYER, PLAYER, PLAYER};

    this->set_shape(3, 3, shape);
    this->_current_event = StateMachine::Event::IDLE;
    this->_event_sem = new Semaphore(1);
}

Player::~Player() {
    if (this->_event_sem) {
        delete this->_event_sem;
        this->_event_sem = nullptr;
    }
}

void Player::init() { this->thread = new Thread(this->update_behaviour, this); }

void Player::update_behaviour(Player *player) {
    while (player->get_health() > 0) {
        player->_event_sem->p();
        StateMachine::Event event = player->_current_event;
        player->_current_event = StateMachine::Event::IDLE;
        player->_event_sem->v();

        player->lock_target_move();
        switch (event) {
            case StateMachine::Event::FORWARD:
                player->move(1, 0);  // TODD: Trocar por target move
                break;
            case StateMachine::Event::BACKWARD:
                player->move(-1, 0);  // TODD: Trocar por target move
                break;
            case StateMachine::Event::RIGHT_TURN:
                player->move(0, 90);  // TODD: Trocar por target move
                break;
            case StateMachine::Event::LEFT_TURN:
                player->move(0, -90);  // TODD: Trocar por target move
                break;
            case StateMachine::Event::SHOOT:
                // player->move();
                break;
            default:
                break;
        }
        player->unlock_target_move();

        Thread::yield();
    }

    player->get_thread()->thread_exit(0);
}

void Player::set_control_event(StateMachine::Event event) {
    this->_event_sem->p();
    this->_current_event = event;
    this->_event_sem->v();
}
