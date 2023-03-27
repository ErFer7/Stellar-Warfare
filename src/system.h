#ifndef system_h
#define system_h

#include <stdio.h>
#include "traits.h"

__BEGIN_API

class System
{
public:
    /*
     * Este método irá realizar a inicialização de todas as variáveis internas do SO.
     * Por enquanto deve apenas desativar o buffer de saída padrão usado pelo cout.
     * setvbuf (stdout, 0, _IONBF, 0) ;
     * Isso evita condições de corrida que podem ocorrer no buffer quando threads são usadas.
     * Deve ser chamado no início da função main.
     */ 
    static void init();

};

__END_API

#endif