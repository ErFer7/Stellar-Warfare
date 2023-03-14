#ifndef main_class_h
#define main_class_h

#include <iostream>
#include "cpu.h"
#include "traits.h"

__BEGIN_API

class Main
{
public:
    Main() {}

    static void run() {
        std::cout << "main: inicio\n";

        ContextMain = new CPU::Context();
        ping_name = "ping";
        pong_name = "pong";

        ping = new CPU::Context(func_ping, (char *) ping_name.data());
        pong = new CPU::Context(func_pong, (char *) pong_name.data());

        CPU::switch_context(ContextMain, ping) ;
        CPU::switch_context(ContextMain, pong) ;

        std::cout << "main: fim\n";

        delete ContextMain;
        delete ping;
        delete pong;
    }

private:

    static const int ITERATIONS = 10;

    static void func_ping(char * arg)
    {
        int i ;

        std::cout << arg << ": inicio\n";

        for (i = 0; i < ITERATIONS; i++)
        {
            std::cout << arg << i << "\n" ;
            CPU::switch_context(ping, pong);
        }
        std::cout << arg << ": fim\n";

        CPU::switch_context(ping, ContextMain);
    }

    static void func_pong(char * arg)
    {
        int i ;

        std::cout << arg << ": inicio\n";

        for (i = 0; i < ITERATIONS; i++)
        {
            std::cout << (char *) arg << i << "\n" ;
            CPU::switch_context(pong, ping) ;
        }
        std::cout << (char *) arg << ": fim\n";

        CPU::switch_context(pong, ContextMain);
    }

    private:
        static CPU::Context *ContextMain;
        static CPU::Context *ping;
        static CPU::Context *pong;

        static std::string ping_name;
        static std::string pong_name;
};

__END_API

#endif