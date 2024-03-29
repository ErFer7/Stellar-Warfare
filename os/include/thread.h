#ifndef thread_h
#define thread_h

#include <chrono>
#include <ctime>

#include "cpu.h"
#include "debug.h"
#include "list.h"
#include "traits.h"

__BEGIN_API

class Thread {
   protected:
    typedef CPU::Context Context;

   public:
    typedef Ordered_List<Thread> Ordered_Queue;

    // Thread State
    enum State { RUNNING, READY, FINISHING, SUSPENDED, WAITING };

    /*
     * Construtor vazio. Necessário para inicialização, mas sem importância para a execução das Threads.
     */
    Thread() {}

    /*
     * Cria uma Thread passando um ponteiro para a função a ser executada
     * e os parâmetros passados para a função, que podem variar.
     * Cria o contexto da Thread.
     * PS: devido ao template, este método deve ser implementado neste mesmo arquivo .h
     */
    template <typename... Tn>
    Thread(void (*entry)(Tn...), Tn... an);

    /*
     * Retorna a Thread que está em execução.
     */
    static Thread *running() { return _running; }

    /*
     * Método para trocar o contexto entre duas thread, a anterior (prev)
     * e a próxima (next).
     * Deve encapsular a chamada para a troca de contexto realizada pela class CPU.
     * Valor de retorno é negativo se houve erro, ou zero.
     */
    static int switch_context(Thread *prev, Thread *next);

    /*
     * Termina a thread.
     * exit_code é o código de término devolvido pela tarefa (ignorar agora, vai ser usado mais tarde).
     * Quando a thread encerra, o controle deve retornar à main.
     */
    void thread_exit(int exit_code);

    /*
     * Retorna o ID da thread.
     */
    int id() { return this->_id; }

    /*
     * NOVO MÉTODO DESTE TRABALHO.
     * Daspachante (disptacher) de threads.
     * Executa enquanto houverem threads do usuário.
     * Chama o escalonador para definir a próxima tarefa a ser executada.
     */
    static void dispatcher();

    /*
     * NOVO MÉTODO DESTE TRABALHO.
     * Realiza a inicialização da class Thread.
     * Cria as Threads main e dispatcher.
     */
    static void init(void (*main)(void *));

    /*
     * Devolve o processador para a thread dispatcher que irá escolher outra thread pronta
     * para ser executada.
     */
    static void yield();

    /*
     * Destrutor de uma thread. Realiza todo os procedimentos para manter a consistência da classe.
     */
    ~Thread();

    /*
     * Retorna o contexto da thread.
     */
    Context *context() { return this->_context; }

    /*
     * Este método suspende a thread em execução até que a thread “alvo” finalize.
     * O inteiro retornado por join() é o argumento recebido por thread_exit(), ou seja, exit_code.
     */
    int join();

    /*
     * Suspende a Thread até que resume() seja chamado.
     */
    void suspend();

    /*
     * Coloca uma Thread que estava suspensa de volta para a fila de prontos.
     */
    void resume();

    /*
     * Coloca uma Thread para esperar até que o semáforo seja liberado.
     */
    static void sleep(Ordered_Queue *sleeping_queue);

    /*
     * Acorda uma Thread que esteva esperando o semáforo.
     */
    static void wakeup(Ordered_Queue *sleeping_queue);

    /*
     * Retorna o _link de uma thread.
     */
    Ordered_Queue::Element *get_link() { return &(this->_link); }

   private:
    int _id;
    Context *volatile _context;
    static Thread *_running;

    static Thread _main;
    static CPU::Context _main_context;
    static Thread _dispatcher;
    static Ordered_Queue _ready;
    Ordered_Queue::Element _link;
    volatile State _state;

    // Atributos adcionados:
    static int _id_counter;
    int _exit_code;
    static Ordered_Queue _suspended;
    Thread *_waiting_join;
};

template <typename... Tn>
Thread::Thread(void (*func)(Tn...), Tn... an)
    : _link(this,
            (std::chrono::duration_cast<std::chrono::microseconds>(
                 std::chrono::high_resolution_clock::now().time_since_epoch())
                 .count())) {
    db<Thread>(TRC) << "Thread constructor called\n";
    this->_context = new Context(func, an...);

    if (this->_context) {
        this->_id = _id_counter++;
        this->_waiting_join = nullptr;

        if (this->_id != 0)  // A thread main não deve ser inserida na fila de prontos
        {
            this->_state = READY;
            _ready.insert(&_link);
        }

        db<Thread>(INF) << "Created Thread with context: " << this->_context << " for thread: " << this->_id << "\n";
    } else {
        db<Thread>(ERR) << "Thread: failed to create context!\n";
        exit(-1);
    }
}

__END_API

#endif