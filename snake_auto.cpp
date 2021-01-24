#include "snake_auto.h"

snakeAuto::snakeAuto(float x, float y, float r, Skin skin, ACL_Image *des): Snake(x, y, r, skin, des) {
    deg = rand() % 360;
}

void snakeAuto::navigate() {
    int I = int(body[0].x) / KERNEL_SIZE;
    int J = int(body[0].y) / KERNEL_SIZE;

    int direction[360 / 5] = {0};

    int score = -(INT_MAX/5);
    int range = 5;

    for (int i = 0; i < 360; i += 5) {
        for (int j = 1; j < range; j++) {
            float x = body[0].x + cos(i*acos(-1.0)/180) * KERNEL_SIZE * j;
            float y = body[0].y - sin(i*acos(-1.0)/180) * KERNEL_SIZE * j;
            if (dangerous(x, y)) {
                direction[int(i) / 5] = -INT_MAX / 4 * (float(j) / range);
            }
        }
    }
    range = 10;
    for (int i = MAX(I - range, 0); i <= MIN(I + range, MAP_WIDTH / KERNEL_SIZE - 1); i++) {
        for (int j = MAX(J - range, 0); j <= MIN(J + range, MAP_HEIGHT / KERNEL_SIZE - 1); j++) {
            if ((*pmap).map[i][j].hasFood()) {
                Food &p = (*pmap).map[i][j];
                float dy = (j * KERNEL_SIZE + p.getY() - body[0].y);
                float dx = (i * KERNEL_SIZE + p.getX() - body[0].x);
                int theta = atan(-dy / dx) / acos(-1.0) * 180 + 360;
                if (dx < 0)
                    theta += 180;
                int bonus = (*pmap).map[i][j].hasBigFood() ? 1000000 : 1000;
                direction[theta % 360 / 5] += bonus * (sqrt(i * i + j * j) / range);
            }
        }
    }

    int deg = this->deg;
    while (deg < 0) {
        deg += INT_MAX / 360 * 360;
    }
    deg %= 360;
    int best_direction = deg;
    for (int i = 0; i < 360 / 5; i++) {
        int rd = rand();
        if (direction[(i-1+rd+360/5) % (360/5)] + direction[(i+rd) % (360/5)] + direction[(i+1+rd) % (360/5)] > score) {
            score = direction[i];
            best_direction = (i + rd) % (360 / 5) * 5;
        }
    }

    int delta = (best_direction + (INT_MAX - best_direction) / 360 * 360 - deg) % 360;
    if (!(delta >= 175 && delta <= 185 && getW() != 0)) {
        if (delta > 180) {
            setW(-5);
        }
        else {
            setW(5);
        }
    }

    if ((delta >= 355 || delta <= 5))
        setW(0);
}