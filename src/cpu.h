#ifndef cpu_h
#define cpu_h

#include <ucontext.h>
#include <iostream>
#include "traits.h"

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

            template<typename ... Tn>
            Context(void (* func)(Tn ...), Tn ... an);

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
CPU::Context::Context(void (*func)(Tn...), Tn... an) {
    getcontext(&this->_context);

    this->_stack = new char[STACK_SIZE];

    if (!this->_stack) {
        exit(-1);  // Caso a alocação do stack falhe
    }

    this->_context.uc_link = 0;
    this->_context.uc_stack.ss_sp = (void *)(this->_stack);
    this->_context.uc_stack.ss_size = STACK_SIZE;
    this->_context.uc_stack.ss_flags = 0;

    makecontext(&this->_context, (void (*)())func, sizeof...(an), an...);
}

__END_API

#endif

