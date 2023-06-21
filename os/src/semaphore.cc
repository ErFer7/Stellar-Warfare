#include <iostream>
#include "../include/traits.h"
#include "../include/semaphore.h"

__BEGIN_API

void Semaphore::p()
{
    db<Semaphore>(TRC) << "Semaphore::p called\n";

    if (CPU::fdec(this->value) < 1)
    {
        db<Semaphore>(INF) << "Semaphore::p: semaphore access denied\n";
        sleep();
    }

    db<Semaphore>(INF) << "Semaphore::p: semaphore access granted\n";
}

void Semaphore::v()
{
    db<Semaphore>(TRC) << "Semaphore::v called\n";

    if (CPU::finc(this->value) < 0)
    {
        wakeup();
    }
}

void Semaphore::sleep()
{
    db<Semaphore>(TRC) << "Semaphore::sleep called\n";

    Thread::sleep(&sleeping_queue);
}

void Semaphore::wakeup()
{
    db<Semaphore>(TRC) << "Semaphore::wakeup called\n";

    Thread::wakeup(&sleeping_queue);
}

void Semaphore::wakeup_all()
{
    db<Semaphore>(TRC) << "Semaphore::wakeup_all called\n";

    int queue_size = this->sleeping_queue.size();
    for (int i = 0; i < queue_size; i++)
    {
        Thread::wakeup(&sleeping_queue);
    }
    // Thread::yield();
}

Semaphore::~Semaphore()
{
    db<Semaphore>(TRC) << "Semaphore::~Semaphore called\n";
    wakeup_all();
}

__END_API