#include "../include/player.h"

#include <iostream>

__USING_API

Player::Player(int x, int y, sf::Texture *texture, float scale, int scene_offset_x, int scene_offset_y)
    : Spaceship(x, y, 0.0f, 32.0f, PLAYER, 3, 2.0f, scale, scene_offset_x, scene_offset_y) {
    this->set_graphics(texture);
    this->_current_event = StateMachine::Event::IDLE;
    this->_event_sem = new Semaphore(1);
    this->thread = new Thread(this->update_behaviour, this);
}

Player::~Player() { delete this->_event_sem; }

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

        if (player->can_move()) {
            switch (event) {
                case StateMachine::Event::UP:
                    player->set_target_move(0, -1);
                    break;
                case StateMachine::Event::DOWN:
                    player->set_target_move(0, 1);
                    break;
                case StateMachine::Event::RIGHT:
                    player->set_target_move(1, 0);
                    break;
                case StateMachine::Event::LEFT:
                    player->set_target_move(-1, 0);
                    break;
                default:
                    break;
            }
        }

        if (player->can_shoot() && event == StateMachine::Event::SPACE) {
            player->shoot();
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
