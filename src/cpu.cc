#include "cpu.h"

#include <iostream>

__BEGIN_API

void CPU::Context::save()
{
    getcontext(&this->_context);
    db<CPU>(TRC) << "CPU::Context::save() called\n";
}

void CPU::Context::load()
{
    if (&this->_context)
    {
        setcontext(&this->_context);
    }
    db<CPU>(TRC) << "CPU::Context::load() called\n";
}

CPU::Context::~Context()
{
    if (this->_stack)
    {
        delete[] this->_stack;
        this->_stack = NULL;
    }
    db<CPU>(TRC) << "CPU::Context destructor called\n";
}

int CPU::switch_context(Context *from, Context *to)
{
    if (!from || !to)
    {
        db<CPU>(ERR) << "CPU::switch_context: from or to is null\n";
        return -1;
    }

    swapcontext(&from->_context, &to->_context);
    db<CPU>(TRC) << "CPU::switch_context: from=" << from << " to=" << to << "\n";
    return 0;
}

__END_API
