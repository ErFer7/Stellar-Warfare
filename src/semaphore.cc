#include <iostream>
#include "traits.h"
#include "semaphore.h"

__BEGIN_API

inline int Semaphore::finc(volatile int &number)
{
    db<Semaphore>(TRC) << "Semaphore::finc called\n";

    int result;
    asm volatile("lock xadd %0, %1"
                 : "=r"(result), "=m"(number)
                 : "0"(1), "m"(number)
                 : "memory");

    db<Semaphore>(INF) << "Semaphore::finc: result =" << result + 1 << "\n";
    return result + 1;
}

inline int Semaphore::fdec(volatile int &number)
{
    db<Semaphore>(TRC) << "Semaphore::fdec called\n";

    int result;
    asm volatile("lock xadd %0, %1"
                 : "=r"(result), "=m"(number)
                 : "0"(-1), "m"(number)
                 : "memory");

    db<Semaphore>(INF) << "Semaphore::fdec: result =" << result - 1 << "\n";
    return result - 1;
}

void Semaphore::p()
{
    db<Semaphore>(TRC) << "Semaphore::p called\n";

    if (fdec(this->value) < 0)
    {
        db<Semaphore>(INF) << "Semaphore::p: semaphore access denied\n";
        sleep();
    }

    db<Semaphore>(INF) << "Semaphore::p: semaphore access granted\n";
}

void Semaphore::v()
{
    db<Semaphore>(TRC) << "Semaphore::v called\n";

    finc(this->value);
    wakeup();
}

void Semaphore::sleep()
{
    db<Semaphore>(TRC) << "Semaphore::sleep called\n";

    Thread *to_sleep = Thread::sleep();
    this->sleeping_queue.insert(to_sleep->get_link());
    Thread::yield();
}

void Semaphore::wakeup()
{
    db<Semaphore>(TRC) << "Semaphore::wakeup called\n";

    if (!this->sleeping_queue.empty())
    {
        Thread *next = this->sleeping_queue.remove()->object();
        Thread::wakeup(next);
    }
}

void Semaphore::wakeup_all()
{
    db<Semaphore>(TRC) << "Semaphore::wakeup_all called\n";

    int queue_size = this->sleeping_queue.size();
    for (int i = 0; i < queue_size; i++)
    {
        Thread *next = this->sleeping_queue.remove()->object();
        Thread::wakeup(next);
    }
}

Semaphore::~Semaphore()
{
    db<Semaphore>(TRC) << "Semaphore::~Semaphore called\n";
    wakeup_all();
}

__END_API