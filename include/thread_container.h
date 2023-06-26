#ifndef thread_container_h
#define thread_container_h

#include "../os/include/thread.h"
#include "../os/include/traits.h"

__USING_API

class ThreadContainer {
   public:
    ThreadContainer();
    ~ThreadContainer();

    inline Thread *get_thread() { return this->thread; }
    inline void join() { this->thread->join(); }
    inline void suspend() { this->thread->suspend(); }
    inline void resume() { this->thread->resume(); }

   protected:
    Thread *thread;
};

#endif