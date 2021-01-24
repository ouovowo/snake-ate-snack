#pragma once
#include "snake_sprite.h"

class snakeUser: public Snake {
public:
    ~snakeUser() {}
    snakeUser() {}
    snakeUser(snakeUser &) {}
    snakeUser(float x, float y, float r, Skin skin, ACL_Image *des);
    void setCenter();
};