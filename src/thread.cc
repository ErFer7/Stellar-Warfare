#include "thread.h"
#include "main_class.h"

__BEGIN_API

// Inicialização das variáveis
int Thread::_id_counter = 0;
Thread *Thread::_running = nullptr;

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
    db<Thread>(TRC) << "Thread::thread_exit called\n";
    delete this->_context;

    db<Thread>(INF) << "Thread::thread_exit: thread " << this->_id << " exited with code " << exit_code << "\n";
    Thread::switch_context(this, Main::mainThread());
}

__END_API
