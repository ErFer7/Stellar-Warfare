#ifndef bullet_h
#define bullet_h

#include "../os/include/traits.h"
#include "entity.h"

__USING_API

class Bullet : public Entity {
   public:
    Bullet() {}
    Bullet(int x,
           int y,
           int rotation,
           Type type,
           sf::Texture* texture,
           float scale,
           int scene_offset_x,
           int scene_offset_y);
    ~Bullet() {}
    void update_behaviour();
};

#endif