#include "cpu.h"

#include <iostream>

__BEGIN_API

void CPU::Context::save() {
    getcontext(&this->_context);
}

void CPU::Context::load() {
    if (&this->_context) {
        setcontext(&this->_context);
    }
}

// TODO: Corrigir as chamadas extras no delete (Teste com o Valgrind)
CPU::Context::~Context() {
    if (this->_stack) {
        delete (this->_stack);
        this->_stack = NULL;
    }
}

int CPU::switch_context(Context *from, Context *to) {
    if (!from || !to) {
        return -1;
    }

    swapcontext(&from->_context, &to->_context);
    return 0;
}

__END_API
