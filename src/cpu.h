#ifndef cpu_h
#define cpu_h

#include <ucontext.h>
#include <iostream>
#include "traits.h"
#include "debug.h"

__BEGIN_API

class CPU
{
public:
    class Context
    {
    private:
        static const unsigned int STACK_SIZE = Traits<CPU>::STACK_SIZE;

    public:
        Context() { _stack = 0; }

        template <typename... Tn>
        Context(void (*func)(Tn...), Tn... an);

        ~Context();

        void save();
        void load();

    private:
        char *_stack;

    public:
        ucontext_t _context;
    };

public:
    static int switch_context(Context *from, Context *to);
};

// O construtor com templates não pode ser definido no arquivo de código fonte
template <typename... Tn>
CPU::Context::Context(void (*func)(Tn...), Tn... an)
{
    db<CPU>(TRC) << "CPU::Context constructor called\n";
    getcontext(&this->_context);

    this->_stack = new char[STACK_SIZE];

    if (!this->_stack)
    {
        db<CPU>(ERR) << "CPU::Context constructor failed to allocate stack\n";
        exit(-1);
    }

    this->_context.uc_link = 0;
    this->_context.uc_stack.ss_sp = (void *)(this->_stack);
    this->_context.uc_stack.ss_size = STACK_SIZE;
    this->_context.uc_stack.ss_flags = 0;

    makecontext(&this->_context, (void (*)())func, sizeof...(an), an...);
    db<CPU>(INF) << "CPU::Context new Context " << this << " created successfully\n";
}

__END_API

#endif
