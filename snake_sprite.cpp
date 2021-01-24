#include "snake_sprite.h"
#include <iostream>
#include <cstring>
#include <ctime>

Map *Snake::pmap = nullptr;
int Snake::counter = 0;
Skin Snake::head;

Snake::~Snake() {
    transfer2map(-1);
    destroy();
}

void Snake::destroy() {
    for (int i = 0; i < int(body.size()); i++) {
        float randn1 = (rand() + rand() + rand() + rand() + rand()) / 5.0 / RAND_MAX * 2 - 1;
        float randn2 = (rand() + rand() + rand() + rand() + rand()) / 5.0 / RAND_MAX * 2 - 1;
        int x = body[i].x + int(randn1 * r * 4);
        int y = body[i].y + int(randn2 * r * 4);
        x = MAX(x, r);
        y = MAX(y, r);
        x = MIN(x, MAP_WIDTH - r - 1);
        y = MIN(y, MAP_HEIGHT - r - 1);
        int X = x / KERNEL_SIZE;
        int Y = y / KERNEL_SIZE;
        x -= X * KERNEL_SIZE;
        y -= Y * KERNEL_SIZE;
        (*pmap).map[X][Y].set(x, y, r, 0, destroyed);
    }
}

Snake::Snake(float x, float y, float r, Skin skin, ACL_Image *des) {
    body.push_back({x, y, 0});
    path.push_back({x, y, 0});
    destroyed = des;
    score = dscore = 0;
    v = 3;
    this->r = r;
    this->skin = skin;
    w = 0;
    deg = 0;
    length = INITIAL_LENGTH;
    id = Snake::counter++;
    for (int i = 0; i < INITIAL_LENGTH; i++)
        add();
}

void Snake::rotateImage(Node node, float r) {
    ACL_Color cmap[100][100];
    clock_t time = clock();
    for (int i = int(node.x-r); i < int(node.x + r); i++) {
        for (int j = int(node.y-r); j < int(node.y + r); j++) {
            cmap[int(i-node.x+r)][int(j-node.y+r)] = getPixel(i, j);
        }
    }
    std::cout<< "getPixel:" << clock() - time << " ";
    float rad = node.deg / 360 * 2 * acos(-1.0);
    float Sin = sin(rad), Cos = cos(rad);
    time = clock();
    for (int i = -r; i < r; i++) {
        for (int j = -r; j < r; j++) {
            if (i * i + j * j < (r - 2) * (r - 2)) {
                int X = int(Cos * i - Sin * j + r);
                int Y = int(Sin * i + Cos * j + r);
                putPixel(int(node.x + i), int(node.y + j), cmap[X][Y]);
            }
        }
    }
    std::cout<< "putPixel:" << clock() - time << " \n";
}

void Snake::paint() {
    float x = (*pmap).getX() - WINDOW_WIDTH / 2;
    float y = (*pmap).getY() - WINDOW_HEIGHT / 2;
    for (int i = body.size() - 1; i; --i) {
        int X = int(body[i].x - x - r);
        int Y = int(body[i].y - y - r);
        if (X >= -2*r && Y >= -2*r && X < WINDOW_WIDTH && Y < WINDOW_HEIGHT)
            putImageTransparent(skin[(i-1) % (skin.size()-1)+1], X, Y, 2*r, 2*r, 0);
    }
    // putImageTransparent(skin[0], int(body[0].x-x-r), int(body[0].y-y-r), 2*r, 2*r, 0);
    // Node node = {body[0].x - x, body[0].y - y, body[0].deg};
    // rotateImage(node, r);
    int theta = deg;
    if (theta < 0) {
        theta += INT_MAX / 360 * 360;
    }
    theta %= 360;
    putImageTransparent(head[theta / 5], int(body[0].x-x-r), int(body[0].y-y-r), 2*r, 2*r, 0);
}

void Snake::add() {
    body.push_back(body[body.size() - 1]);
}

// +-------->x
// |
// |
// |
// v y

void Snake::eat() {
    const int range = 60;
    int up     = MAX(int(body[0].y - (r  + range)), 0);
    int bottom = MIN(int(body[0].y + (r  + range)), MAP_HEIGHT - 1);
    int left   = MAX(int(body[0].x - (r  + range)), 0);
    int right  = MIN(int(body[0].x + (r  + range)), MAP_WIDTH - 1);
    for (int i = left / KERNEL_SIZE; i <= right / KERNEL_SIZE; i++) {
        for (int j = up / KERNEL_SIZE; j <= bottom / KERNEL_SIZE; j++) {
            int x = i * KERNEL_SIZE;
            int y = j * KERNEL_SIZE;
            if ((*pmap).map[i][j].hasFood() &&
                ((x + (*pmap).map[i][j].getX() - body[0].x) * (x + (*pmap).map[i][j].getX() - body[0].x) +
                (y + (*pmap).map[i][j].getY() - body[0].y) * (y + (*pmap).map[i][j].getY() - body[0].y)) <
                (r  + range) * (r  + range)
            ) {
                if (((*pmap).map[i][j].hasBigFood())) {
                    score += 3;
                    dscore += 3;
                }
                else {
                    score += 1;
                    dscore += 1;
                }
                while (dscore > 15) {
                    dscore -= 15;
                    add();
                }
                float dx = +v * cos(deg / 360.0 * 2 * acos(-1.0)) * DISAPPEAR_DELAY;
                float dy = -v * sin(deg / 360.0 * 2 * acos(-1.0)) * DISAPPEAR_DELAY;
                (*pmap).map[i][j].erase(body[0].x + dx, body[0].y + dy);
            }
        }
    }
}

void Snake::move(float dt) {
    transfer2map(-1);
    float dx = +v * cos(deg / 360.0 * 2 * acos(-1.0)) * dt;
    float dy = -v * sin(deg / 360.0 * 2 * acos(-1.0)) * dt;
    float ds = +v * dt;
    float x = body[0].x;
    float y = body[0].y;
    path.push_front({x + dx, y + dy, deg});
    int length = body.size();
    int i = 0;
    auto it = path.begin();
    float s = 0;
    for (; it != path.end(); it++) {
        if (i < length) {
            if (s >= i * r - 0.01) {
                body[i] = *it;
                i++;
            }
        }
        else {
            break;
        }
        s += ds;
    }
    while (it != path.end()) {
        path.pop_back();
    }
    for (int j = i; j < length; j++) {
        body[j] = path.back();
    }
    eat();
    setCenter();
    transfer2map(id);
}

void Snake::rotate(float dt) {
    deg += w * dt;
}

std::pair<int, int> Snake::getHead() {
    return std::pair<int, int>({int(body[0].x), int(body[0].y)});
}

void Snake::setMap(Map *m) {
    pmap = m;
}

bool Snake::dead() {
    return dangerous(body[0].x, body[0].y);
}

bool Snake::dangerous(float x, float y) {
    if (x - r < 0 || y - r < 0 || x + r >= MAP_WIDTH || y + r >= MAP_HEIGHT)
        return true;
    int up     = MAX(int(y - r), 0);
    int bottom = MIN(int(y + r), MAP_HEIGHT - 1);
    int left   = MAX(int(x - r), 0);
    int right  = MIN(int(x + r), MAP_WIDTH - 1);
    for (int i = left / KERNEL_SIZE; i <= right / KERNEL_SIZE; i++) {
        for (int j = up / KERNEL_SIZE; j <= bottom / KERNEL_SIZE; j++) {
            int X = i * KERNEL_SIZE;
            int Y = j * KERNEL_SIZE;
            if ((*pmap).sMap[i][j].id != -1 && (*pmap).sMap[i][j].id != id &&
                ((X + (*pmap).sMap[i][j].x - x) * (X + (*pmap).sMap[i][j].x - x) +
                 (Y + (*pmap).sMap[i][j].y - y) * (Y + (*pmap).sMap[i][j].y - y)) < (r*2) * (r*2)) {
                return true;
            }
        }
    }
    return false;
}

void Snake::transfer2map(int id) {
    for (int i = 0; i < int(body.size()); i++) {
        int x = int(body[i].x) / KERNEL_SIZE;
        int y = int(body[i].y) / KERNEL_SIZE;
        (*pmap).sMap_[x][y] = {body[i].x - x * KERNEL_SIZE, body[i].y - y * KERNEL_SIZE, id};
    }
}

void Snake::loadHead() {
    for (int i = 0; i < 360; i += 5) {
        ACL_Image *img = new ACL_Image();
        char path[20];
        sprintf(path, "head/%d.gif", i);
        loadImage(path, img);
        head.push_back(img);
    }
}