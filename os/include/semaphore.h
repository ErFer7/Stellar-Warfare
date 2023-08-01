#ifndef semaphore_h
#define semaphore_h

#include "cpu.h"
#include "debug.h"
#include "thread.h"
#include "traits.h"

__BEGIN_API

class Semaphore {
   public:
    typedef Ordered_List<Thread> Ordered_Queue;

    Semaphore(int v = 1) { this->value = v; }
    ~Semaphore();

    void p();
    void v();

   private:
    // Atomic operations
    int finc(volatile int &number);
    int fdec(volatile int &number);

    // Thread operations
    void sleep();
    void wakeup();
    void wakeup_all();

   private:
    // Added attributes
    volatile int value;
    Ordered_Queue sleeping_queue;
};

__END_API

#endif
