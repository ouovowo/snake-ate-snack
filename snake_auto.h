#pragma once
#include "snake_sprite.h"

class snakeAuto: public Snake {
public:
    ~snakeAuto() {}
    snakeAuto() {}
    snakeAuto(snakeAuto &) {}
    snakeAuto(float x, float y, float r, Skin skin, ACL_Image *des);
    void navigate();
};