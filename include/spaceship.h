#ifndef spaceship_h
#define spaceship_h

#include "../os/include/thread.h"
#include "../os/include/traits.h"

__USING_API

class Spaceship : public Thread {
   public:
    Spaceship();
    ~Spaceship();

   private:
};

#endif