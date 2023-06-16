#include "../include/player.h"

#include <iostream>

__USING_API

Player::Player(int x, int y, sf::Texture *texture, sf::Color color, float scale, float speed)
    : Spaceship(x, y, 0.0f, texture, color, scale, speed) {
    Type shape[] = {VOID, PLAYER, VOID, PLAYER, PLAYER, PLAYER, PLAYER, PLAYER, PLAYER};

    this->set_shape(3, 3, shape);
    this->thread = new Thread(this->update_behaviour, this);
}

Player::~Player() {}

void Player::update_behaviour(Player *player) {
    while (player->get_health() > 0) {
        std::cout << "Player health: " << player->get_health() << std::endl;
        Thread::yield();
    }

    player->get_thread()->thread_exit(0);
}
