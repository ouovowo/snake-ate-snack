#include "snake_food.h"

Food::Food() {
    x = y = r = color = 0;
    eaten = false;
    img = nullptr;
}

void Food::set(int x, int y, int r, COLORREF color, ACL_Image *img) {
    this->x = x;
    this->y = y;
    this->r = r;
    this->color = color;
    this->img = img;
}

void Food::sowing(float p) {
    if (color == 0 && img == nullptr) {
        unsigned int r1 = rand();
        unsigned int r2 = rand();
        unsigned int r3 = rand();
        unsigned int r4 = rand();

        if (float(r1)/RAND_MAX < p) {
            color = color_collection[r2 % COLOR_NUMBER];
            x = int(float(r3 - 0.1)/RAND_MAX * KERNEL_SIZE);
            y = int(float(r4 - 0.1)/RAND_MAX * KERNEL_SIZE);
            r = 4;
        }
    }
}

bool Food::hasFood() {
    if (!eaten && (img != nullptr || color != 0))
        return true;
    return false;
}


bool Food::hasBigFood() {
    if (!eaten && img != nullptr)
        return true;
    return false;
}

void Food::paint(int X, int Y, int left, int up) {
    int R = r;
    int dx = x;
    int dy = y;
    if (eaten) {
        float rate = float(t) / DISAPPEAR_DELAY;
        R = int(r * (1 - rate*rate) + 0.5);
        dx = int(x + (aimX - left - x) * rate*rate + 0.5);
        dy = int(y + (aimY - up - y) * rate*rate + 0.5);
    }
    if (color != 0) {
        setPenColor(color);
        setBrushColor(color);
        ellipse(X + dx - R, Y + dy - R, X + dx + R, Y + dy + R);
    }
    if (img != nullptr)
        putImageTransparent(img, X + dx - R, Y + dy - R, 2 * R, 2 * R, 0);
}

void Food::update() {
    if (eaten) {
        if (++t > DISAPPEAR_DELAY) {
            eaten = false;
            img = nullptr;
            color = 0;
        }
    }
}