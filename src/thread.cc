#include "thread.h"

__BEGIN_API

// Inicialização das variáveis
int Thread::_id_counter = 0;
Thread *Thread::_running = nullptr;

int Thread::switch_context(Thread *prev, Thread *next)
{
    if (prev == nullptr || next == nullptr)
    {
        db<Thread>(ERR) << "switch_context: prev or next is null\n";
        return -1;
    }
    _running = next;
    db<Thread>(TRC) << "switch_context: prev=" << prev->_id << " next=" << next->_id << "\n";
    return CPU::switch_context(prev->_context, next->_context);
}

void Thread::thread_exit(int exit_code)
{
    this->_context->~Context();
    delete _context;
    // give control to main?
}

__END_API
