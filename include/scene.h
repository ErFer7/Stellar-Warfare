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
    inline Thread *get_thread() { return this->_thread; }

   private:
    static void update_scene(Scene *scene);

   private:
    Thread *_thread;
};

#endif