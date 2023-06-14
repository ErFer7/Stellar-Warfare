#include "../include/player.h"

__USING_API

Player::Player(int x, int y, sf::Texture *texture) : Spaceship(x, y, 0.0f, texture) {}

Player::~Player() {}
