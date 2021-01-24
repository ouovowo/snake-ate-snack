#pragma once
#include "acllib.h"
#include "snake_food.h"

const int WINDOW_HEIGHT = 700;
const int WINDOW_WIDTH = 1300;

const int MAP_HEIGHT = 3 * WINDOW_HEIGHT;
const int MAP_WIDTH  = 3 * WINDOW_WIDTH;

struct Info {
    float x, y;
    int id;
};

class Map {
private:
    ACL_Image *edge;
    ACL_Image *texture;
    float centerX, centerY;
public:
    Food map[MAP_WIDTH / KERNEL_SIZE][MAP_HEIGHT / KERNEL_SIZE];
    Info sMap[MAP_WIDTH / KERNEL_SIZE][MAP_HEIGHT / KERNEL_SIZE];
    Info sMap_[MAP_WIDTH / KERNEL_SIZE][MAP_HEIGHT / KERNEL_SIZE];
    void sowingGlobal(float p);
    Map();
    Map(Map &map);
    Map(ACL_Image *texture, ACL_Image *edge);
    void setTexture(ACL_Image *texture, ACL_Image *edge);
    void setCenter(float x, float y);
    void display();
    float getX() { return centerX; }
    float getY() { return centerY; }
    void refresh();
};