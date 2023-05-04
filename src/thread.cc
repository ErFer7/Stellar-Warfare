#include "thread.h"
#include "main_class.h"

__BEGIN_API

// Inicialização das variáveis
int Thread::_id_counter = 0;
Thread *Thread::_running = nullptr;
Thread::Ready_Queue Thread::_ready;
Thread Thread::_dispatcher;
Thread Thread::_main;
CPU::Context Thread::_main_context;

int Thread::switch_context(Thread *prev, Thread *next)
{
    db<Thread>(TRC) << "Thread::switch_context called\n";

    if (!prev || !next)
    {
        db<Thread>(ERR) << "Thread::switch_context: prev or next is null\n";
        return -1;
    }

    _running = next;

    db<Thread>(INF) << "Thread::switch_context: from =" << prev->_id << " to =" << next->_id << "\n";
    return CPU::switch_context(prev->_context, next->_context);
}

void Thread::thread_exit(int exit_code)
{
    db<Thread>(TRC) << "Thread::thread_exit called by thread" << _running->_id << " with code " << exit_code << "\n";
    this->_state = FINISHING;
    yield();
}

void Thread::dispatcher()
{
    db<Thread>(TRC) << "Thread::dispatcher called\n";

    while(_ready.size() > 0)
    {
        Thread *next = _ready.remove()->object();
        db<Thread>(INF) << "Thread::dispatcher: the next thread is " << next->_id << "\n";

        _dispatcher._state = READY;

        _running = next;
        _running->_state = RUNNING;

        switch_context(&_dispatcher, _running);

        if (_running->_state == FINISHING)
        {
            db<Thread>(INF) << "Thread::dispatcher: thread " << _running->_id << " finished\n";
            _ready.remove(_running);
        }
    }

    db<Thread>(INF) << "Thread::dispatcher: no more threads to run\n";
    _dispatcher._state = FINISHING;

    db<Thread>(INF) << "Thread::dispatcher: exiting\n";
    switch_context(&_dispatcher, &_main);
}

void Thread::init(void (*main)(void *))
{
    db<Thread>(TRC) << "Thread::init called\n";

    new (&_main_context) CPU::Context();

    new (&_ready) Ready_Queue();
    new (&_main) Thread(main, (void*)"main");
    new (&_dispatcher) Thread(dispatcher);

    _running = &_main;
    _main._state = RUNNING;

    db<Thread>(TRC) << "Thread::init switching to main\n";
    CPU::switch_context(&_main_context, _main.context());
}

void Thread::yield()
{
    db<Thread>(TRC) << "Thread::yield called by thread " << _running->_id << "\n";

    if(_ready.empty())
    {
        db<Thread>(INF) << "Thread::dispatcher: ready queue is empty\n";
        return;
    }

    if (_running->_state != FINISHING && _main._id != _running->_id)
    {
        int now = std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::high_resolution_clock::now().time_since_epoch()).count();
        _running->_link.rank(now);
        _ready.insert(&_running->_link);
    }

    _running->_state = READY; 

    Thread *_previous = _running;

    _running = &_dispatcher;
    _running->_state = RUNNING;

    db<Thread>(INF) << "Thread::yield: switching from " << _previous->_id << " to " << _running->_id << "\n";
    switch_context(_previous, _running);
}

Thread::~Thread()
{
    db<Thread>(TRC) << "Destructor called by thread " << this->_id << "\n";
    delete this->_context;
}

__END_API
