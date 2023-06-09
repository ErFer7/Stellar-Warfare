#include "../include/thread.h"

__BEGIN_API

// Inicialização das variáveis
int Thread::_id_counter = 0;
Thread *Thread::_running = nullptr;
Thread::Ordered_Queue Thread::_ready;
Thread Thread::_dispatcher;
Thread Thread::_main;
CPU::Context Thread::_main_context;
Thread::Ordered_Queue Thread::_suspended;

int Thread::switch_context(Thread *prev, Thread *next)
{
    db<Thread>(TRC) << "Thread::switch_context called\n";

    if (!prev || !next)
    {
        db<Thread>(ERR) << "Thread::switch_context: prev or next is null\n";
        return -1;
    }

    _running = next;

    db<Thread>(INF) << "Thread::switch_context: from =" << prev->id() << " to =" << next->id() << "\n";
    return CPU::switch_context(prev->_context, next->_context);
}

void Thread::thread_exit(int exit_code)
{
    db<Thread>(TRC) << "Thread::thread_exit called by thread" << _running->id() << " with code " << exit_code << "\n";

    this->_state = FINISHING;
    this->_exit_code = exit_code;

    Thread *_waiting = this->_waiting_join;

    if (_waiting)
    {
        if (_waiting->id() != _main.id())
        {
            _waiting->resume();
        }
        else
        {
            db<Thread>(INF) << "Thread::thread_exit: resuming main thread\n";
            _main._state = RUNNING;
            switch_context(this, &_main);
        }
    }

    yield();
}

void Thread::dispatcher()
{
    db<Thread>(TRC) << "Thread::dispatcher called\n";

    while (_ready.size())
    {
        Thread *next = _ready.remove()->object();
        db<Thread>(INF) << "Thread::dispatcher: the next thread is " << next->id() << "\n";

        _dispatcher._state = READY;
        _ready.insert(&_dispatcher._link);

        _running = next;
        _running->_state = RUNNING;

        switch_context(&_dispatcher, _running);

        if (_running->_state == FINISHING)
        {
            db<Thread>(INF) << "Thread::dispatcher: thread " << _running->id() << " finished\n";
            _ready.remove(_running);
        }
    }

    db<Thread>(INF) << "Thread::dispatcher: no more threads to run\n";
    _dispatcher._state = FINISHING;
    _ready.remove(&_dispatcher._link);

    db<Thread>(INF) << "Thread::dispatcher: exiting\n";
    switch_context(&_dispatcher, &_main);
}

void Thread::init(void (*main)(void *))
{
    db<Thread>(TRC) << "Thread::init called\n";

    new (&_main) Thread(main, (void *)"main");
    new (&_dispatcher) Thread(&dispatcher);

    _running = &_main;
    _main._state = RUNNING;

    CPU::switch_context(&_main_context, _main.context());
}

void Thread::yield()
{
    db<Thread>(TRC) << "Thread::yield called by thread " << _running->id() << "\n";

    Thread *next = _ready.remove()->object();

    if (_running->_state == RUNNING)
    {
        if (_main.id() != _running->id())
        {
            int now = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
            _running->_link.rank(now);
            _ready.insert(&_running->_link);
        }

        _running->_state = READY;
    }

    Thread *_previous = _running;

    _running = next;
    _running->_state = RUNNING;
    _ready.remove(_running);

    db<Thread>(INF) << "Thread::yield: switching from " << _previous->id() << " to " << _running->id() << "\n";
    switch_context(_previous, _running);
}

Thread::~Thread()
{
    delete this->_context;
}

int Thread::join()
{
    db<Thread>(TRC) << "Thread::join called for thread " << this->id() << "\n";

    if (this->_state != FINISHING)
    {
        this->_waiting_join = _running;
        _running->suspend();
    }
    else
    {
        db<Thread>(ERR) << "Thread::join: thread " << this->id() << " is already finishing\n";
        return -1;
    }

    return _exit_code;
}

void Thread::suspend()
{
    db<Thread>(TRC) << "Thread::suspend called for thread " << this->id() << "\n";

    if (this->_state != SUSPENDED)
    {
        this->_state = SUSPENDED;
        _suspended.insert(&this->_link);

        if (this != _running)
        {
            _ready.remove(this);
        }
        else
        {
            yield();
        }
    }
    else
    {
        db<Thread>(WRN) << "Thread::suspend: thread " << this->id() << " is already suspended\n";
    }
}

void Thread::resume()
{
    db<Thread>(TRC) << "Thread::resume called for thread " << this->id() << "\n";

    if (this->_state == SUSPENDED)
    {
        _suspended.remove(&this->_link);
        _ready.insert(&this->_link);
        this->_state = READY;
    }
    else
    {
        db<Thread>(WRN) << "Thread::resume: thread " << this->id() << " is not suspended\n";
    }
}

Thread *Thread::sleep()
{
    db<Thread>(TRC) << "Thread::sleep called for thread " << _running->id() << "\n";

    _running->_state = WAITING;
    return _running;
}

void Thread::wakeup(Thread *next)
{
    db<Thread>(TRC) << "Thread::wakeup called for thread " << next->id() << "\n";

    next->_state = READY;
    _ready.insert(&(next->_link));
}

__END_API