#ifndef entity_h
#define entity_h

#include <SFML/Graphics.hpp>

#include "../os/include/traits.h"
#include <vector>

__USING_API

class Entity {
   public:
    Entity() {}
    Entity(int x, int y, float rotation, sf::Texture *texture);
    ~Entity();
    void update_behaviour() {}  // TODO: Verificar se deve ser virtual
    void render(sf::RenderWindow *window);

   private:
    int _position[2];
    float _rotation;
    sf::Sprite _sprite;
};

#endif
