#include "../include/widget.h"

__USING_API

Widget::Widget(Alignment alignment, int x, int y, int window_width, int window_height) {
    switch (alignment)
    {
    case TOP_LEFT:
        this->position[0] = x;
        this->position[1] = y;
        break;
    case TOP:
        this->position[0] = x + window_width / 2;
        this->position[1] = y;
        break;
    case TOP_RIGHT:
        this->position[0] = x + window_width;
        this->position[1] = y;
        break;
    case LEFT:
        this->position[0] = x;
        this->position[1] = y + window_height / 2;
        break;
    case CENTER:
        this->position[0] = x + window_width / 2;
        this->position[1] = y + window_height / 2;
        break;
    case RIGHT:
        this->position[0] = x + window_width;
        this->position[1] = y + window_height / 2;
        break;
    case BOTTOM_LEFT:
        this->position[0] = x;
        this->position[1] = y + window_height;
        break;
    case BOTTOM:
        this->position[0] = x + window_width / 2;
        this->position[1] = y + window_height;
        break;
    case BOTTOM_RIGHT:
        this->position[0] = x + window_width;
        this->position[1] = y + window_height;
        break;
    default:
        break;
    }
}
