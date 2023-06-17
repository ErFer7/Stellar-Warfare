#ifndef thread_container_h
#define thread_container_h

#include "../os/include/traits.h"
#include "../os/include/thread.h"

__USING_API

class ThreadContainer {
   public:
    ThreadContainer();
    ~ThreadContainer();
    virtual void init() {}
    void join();
    inline Thread *get_thread() { return this->thread; }

   protected:
    Thread *thread;
};

#endif