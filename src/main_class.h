#ifndef main_class_h
#define main_class_h

#include <iostream>
#include "cpu.h"
#include "traits.h"
#include "thread.h"

__BEGIN_API

class Main
{
public:
    Main() {
        main_name = "main";
        ping_name = "    Ping";
        pong_name = "        Pong";
        ThreadMain = new Thread(run, (char *) main_name.data());
        ping = new Thread(func_ping, (char *) ping_name.data());
        pong = new Thread(func_pong, (char *) pong_name.data());
        
        mainContext = new CPU::Context();

        CPU::switch_context(mainContext, ThreadMain->context());

        delete mainContext;
    }

    static void run(char * arg) {
        std::cout << arg << ": inicio\n";

        Thread::switch_context(ThreadMain, ping);
        Thread::switch_context(ThreadMain, pong);

        std::cout << arg << ": fim\n";

        CPU::switch_context(ThreadMain->context(), mainContext);
    }

    ~Main() {
        ThreadMain->thread_exit(0);
        ping->thread_exit(0);
        pong->thread_exit(0);
        delete ThreadMain;
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
            Thread::switch_context(ping, pong);
        }
        std::cout << arg << ": fim\n";

        Thread::switch_context(ping, ThreadMain);
    }

    static void func_pong(char * arg)
    {
        int i ;

        std::cout << arg << ": inicio\n";

        for (i = 0; i < ITERATIONS; i++)
        {
            std::cout << arg << i << "\n" ;
            Thread::switch_context(pong, ping);
        }
        std::cout << arg << ": fim\n";

        Thread::switch_context(pong, ThreadMain);
    }

    private:
        static Thread *ThreadMain;
        static Thread *ping;
        static Thread *pong;
        static CPU::Context *mainContext;

        std::string main_name;
        std::string ping_name;
        std::string pong_name;
};

__END_API

#endif