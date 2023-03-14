#include "cpu.h"
#include <iostream>

__BEGIN_API

// Adicionei este construtor aqui porque acho que o professor esqueceu de adicionar
template<typename ... Tn>
CPU::Context::Context(void (* func)(Tn ...), Tn ... an)
{

}

void CPU::Context::save()
{
    //adicionar implementação
}

void CPU::Context::load()
{
    //adicionar implementação
}

CPU::Context::~Context()
{
    //adicionar implementação
}

int CPU::switch_context(Context *from, Context *to)
{
    //implementação do método
}

__END_API
