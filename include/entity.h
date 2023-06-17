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
    Entity(int x, int y, int rotation, float speed, int scale);
    ~Entity();
    void render(sf::RenderWindow *window);
    void move(int direction, int rotation);
    void set_target_move(int direction, int rotation);
    inline int *get_position() { return this->_position; }
    inline int get_rotation() { return this->_rotation; }
    inline int *get_target_position() { return this->_target_position; }
    inline int get_target_rotation() { return this->_target_rotation; }
    inline bool has_target_movement() { return this->_has_target_movement; }

   protected:
    void set_graphics(sf::Texture *texture, sf::Color color);
    void set_shape(int width, int height, Type *type);

   private:
    void update_sprite();

   private:
    int _position[2];
    int _rotation;
    int _target_position[2];
    int _target_rotation;
    bool _has_target_movement;
    sf::Sprite _sprite;  // TODO: Deveria ser alocado no heap?
    float _speed;
    Matrix<Type> *_shape;
    int _scale;
};

#endif
