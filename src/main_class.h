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
    }

    static void run(void * name) {
        std::cout << (char *) name << ": inicio\n";

        std::string pang_name = "   Pang";
        std::string peng_name = "       Peng";
        std::string ping_name = "           Ping";
        std::string pong_name = "               Pong";
        std::string pung_name = "                   Pung";

        ping_pong_threads[0] = new Thread(body, (char *) pang_name.data(), 0);
        ping_pong_threads[1] = new Thread(body, (char *) peng_name.data(), 1);
        ping_pong_threads[2] = new Thread(body, (char *) ping_name.data(), 2);
        ping_pong_threads[3] = new Thread(body, (char *) pong_name.data(), 3);
        ping_pong_threads[4] = new Thread(body, (char *) pung_name.data(), 4);

        Thread::yield();

        std::cout << (char *) name << ": fim\n";

        delete ping_pong_threads[0];
        delete ping_pong_threads[1];
        delete ping_pong_threads[2];
        delete ping_pong_threads[3];
        delete ping_pong_threads[4];
    }

    ~Main() {}

private:

    static const int ITERATIONS = 10;

    static void body(char *name, int id)
    {
        int i ;

        std::cout << name << ": inicio\n";

        for (i = 0; i < ITERATIONS; i++)
        {
            std::cout << name << ": " << i << "\n" ;
            Thread::yield();
        }
        std::cout << name << ": fim\n";


        ping_pong_threads[id]->thread_exit(0);
    }

    private:
        static Thread *ping_pong_threads[5];
};

__END_API

#endif
