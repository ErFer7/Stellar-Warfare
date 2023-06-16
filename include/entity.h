#ifndef entity_h
#define entity_h

#include <SFML/Graphics.hpp>
#include <vector>

#include "../os/include/traits.h"
#include "matrix.h"

__USING_API

class Entity {
   public:
    enum Type { VOID = 0b0000, PLAYER = 0b0001, ENEMY = 0b0010, PLAYER_BULLET = 0b0100, ENEMY_BULLET = 0b1000 };

    Entity() { this->_shape = nullptr; }
    Entity(int x, int y, float rotation, float speed, int scale);
    ~Entity();
    void render(sf::RenderWindow *window);

   protected:
    void set_graphics(sf::Texture *texture, sf::Color color);
    void set_shape(int width, int height, Type *type);

   private:
    int _position[2];
    float _rotation;
    sf::Sprite _sprite;  // TODO: Deveria ser alocado no heap?
    float _speed;
    Matrix<Type> *_shape;
    int _scale;
};

#endif
