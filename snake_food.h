#pragma once
#include "acllib.h"
#include <cstdlib>
#include <cmath>

const int DISAPPEAR_DELAY = 5;
const int KERNEL_SIZE = 30;
const int COLOR_NUMBER = 3;
const COLORREF color_collection[COLOR_NUMBER] = {
    0xffd042, 0x3877ad, 0xce16d9
};

class Food {
private:
    int t;
    bool eaten;
    int x, y, r;
    float aimX, aimY;
    COLORREF color;
    ACL_Image *img;
public:
    Food();
    Food(Food &) {}
    void set(int x, int y, int r, COLORREF color, ACL_Image *img);
    void sowing(float p);
    bool hasFood();
    bool hasBigFood();
    void paint(int x, int y, int left, int up);
    int getX() { return x; }
    int getY() { return y; }
    void update();
    void erase(float x, float y) {
        eaten = true;
        aimX = x;
        aimY = y;
        t = 0;
    }
};