#ifndef renderer_h
#define renderer_h

#include "../os/include/traits.h"
#include "../os/include/thread.h"

__USING_API

class Renderer : public Thread {
   public:
    Renderer();
    ~Renderer();

   private:
};

#endif