#ifndef scene_h
#define scene_h

#include "../os/include/thread.h"
#include "../os/include/traits.h"
#include "game.h"

__USING_API

class Scene {
   public:
    Scene();
    ~Scene();
    void init();
    void stop();

   private:
    static void UpdateScene();

   private:
    Thread *_thread;
};

#endif