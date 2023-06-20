#include "../include/player.h"

#include <iostream>

__USING_API

Player::Player(int x, int y, sf::Texture *texture) : Spaceship(x, y, 0.0f, 8.0f, PLAYER, 3) {
    bool shape[] = {false, true, false, true, true, true, true, true, true};

    this->set_graphics(texture);
    this->set_shape(3, 3, shape);
    this->_current_event = StateMachine::Event::IDLE;
    this->_event_sem = new Semaphore(1);
    this->thread = new Thread(this->update_behaviour, this);
}

Player::~Player() {
    if (this->_event_sem) {
        delete this->_event_sem;
        this->_event_sem = nullptr;
    }
}

void Player::update_behaviour(Player *player) {
    while (true) {
        player->_event_sem->p();
        StateMachine::Event event = player->_current_event;
        player->_current_event = StateMachine::Event::IDLE;
        player->_event_sem->v();

        player->lock();
        if (player->get_health() <= 0) {
            player->unlock();
            break;
        }

        if (!player->can_move()) {
            player->unlock();
            Thread::yield();
            continue;
        }

        switch (event) {
            case StateMachine::Event::UP:
                player->set_target_move(1, 0);
                break;
            case StateMachine::Event::DOWN:
                player->set_target_move(-1, 0);
                break;
            case StateMachine::Event::RIGHT:
                player->set_target_move(0, 90);
                break;
            case StateMachine::Event::LEFT:
                player->set_target_move(0, -90);
                break;
            case StateMachine::Event::SPACE:
                player->shoot();
                break;
            default:
                break;
        }
        player->unlock();

        Thread::yield();
    }

    player->get_thread()->thread_exit(0);
}

void Player::set_control_event(StateMachine::Event event) {
    this->_event_sem->p();
    this->_current_event = event;
    this->_event_sem->v();
}
