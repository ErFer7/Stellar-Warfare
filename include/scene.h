#ifndef scene_h
#define scene_h

#include "../os/include/traits.h"
#include "../os/include/thread.h"

__BEGIN_API

class Scene : public Thread {
   public:
    Scene();
    ~Scene();

   private:
};

__END_API

#endif