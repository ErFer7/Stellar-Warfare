#include "thread.h"

__BEGIN_API

int Thread::switch_context(Thread * prev, Thread * next){
    return CPU::switch_context(prev->_context, next->_context);
}

void Thread::thread_exit(int exit_code){
    this->_context->~Context();
    delete _context;
    //give control to main?
}

int Thread::id(){
    return this->_id;
}

__END_API