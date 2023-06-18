#include "../include/spaceship.h"

__USING_API

Spaceship::Spaceship(int x, int y, float rotation, float speed) : Entity(x, y, rotation, speed) {
    this->_health = 1;
    this->_target_move_sem = new Semaphore(1);
}

Spaceship::~Spaceship() {
    if (this->_target_move_sem) {
        delete this->_target_move_sem;
        this->_target_move_sem = nullptr;
    }
}
