#include "thread.h"

__BEGIN_API

// Inicialização das variáveis
int Thread::_id_counter = 0;
Thread *Thread::_running = nullptr;

int Thread::switch_context(Thread *prev, Thread *next)
{
    // O valor de retorno já está implementado em switch_context, mas isso aqui é ok
    if (prev == nullptr || next == nullptr)  // Poderia ser (!prev || !next). Mas isso é só detalhe
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
    delete this->_context;

    // give control to main?

    // Acredito que rentornar o controle para a main seja redefinir _running
    // mas não tenho certeza

    // _running = nullptr;  // Talvez isso?

    // Acho que isso também seja necessário
    _id_counter--;
}

__END_API
