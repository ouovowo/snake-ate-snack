#include "snake_user.h"

void snakeUser::setCenter() {
    (*pmap).setCenter(body[0].x, body[0].y);
}

snakeUser::snakeUser(float x, float y, float r, Skin skin, ACL_Image *des): Snake(x, y, r, skin, des) {
    if (x - r < 0)
        body[0].x = r;
    if (y - r < 0)
        body[0].y = r;
    if (x + r >= MAP_WIDTH)
        body[0].x = MAP_WIDTH - r - 1;
    if (y + r >= MAP_HEIGHT)
        body[0].y = MAP_HEIGHT - r - 1;
    float dy = MAP_HEIGHT / 2 - y;
    float dx = MAP_WIDTH / 2 - x;
    if (dx == 0) {
        if (dy > 0)
            deg = -90;
        if (dy < 0)
            deg = 90;
    }
    else {
        deg = atan(-dy / dx) / acos(-1.0) * 180 + 360;
        if (dx < 0)
            deg += 180;
    }
}