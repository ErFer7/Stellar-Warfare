#include "../include/thread_container.h"

__USING_API

ThreadContainer::ThreadContainer() { this->thread = nullptr; }

ThreadContainer::~ThreadContainer() {
    if (this->thread) {
        delete this->thread;
        this->thread = nullptr;
    }
}

void ThreadContainer::join() { this->thread->join(); }
