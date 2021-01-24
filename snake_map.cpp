#include "snake_map.h"
#include <iostream>
#include <cstdlib>
#include <ctime>

Map::Map() {
    memset(sMap, -1, sizeof(sMap));
}

Map::Map(Map &map) {
    centerX = MAP_WIDTH / 2;
    centerY = MAP_HEIGHT / 2;
    this->texture = map.texture;
    this->edge = map.edge;
    memset(sMap, -1, sizeof(sMap));
}

Map::Map(ACL_Image *texture, ACL_Image *edge){
    centerX = MAP_WIDTH / 2;
    centerY = MAP_HEIGHT / 2;
    this->texture = texture;
    this->edge = edge;
}

void Map::setTexture(ACL_Image *texture, ACL_Image *edge) {
    this->texture = texture;
    this->edge = edge;
    sowingGlobal(0.1);
}

void Map::display() {
    clock_t time = clock();
    int left = centerX - WINDOW_WIDTH / 2;
    int up = centerY - WINDOW_HEIGHT / 2;
    for (int i = -left % 100 - 100; i < WINDOW_WIDTH; i += 100) {
        for (int j = -up % 100 - 100; j < WINDOW_HEIGHT; j += 100) {
            putImageScale(texture, i, j, 100, 100);
        }
    }
    std::cout << "o:" << clock() - time << " ";
    time = clock();
    if (left < 0) {
        for (int i = -left - 100; i > -100; i -= 100) {
            for (int j = -up % 100 - 100; j < WINDOW_HEIGHT; j += 100) {
                putImageScale(edge, i, j, 100, 100);
            }
        }
    }
    std::cout << "<:" << clock() - time << " ";
    time = clock();
    if (up < 0) {
        for (int i = -left % 100 - 100; i < WINDOW_WIDTH; i += 100) {
            for (int j = -up - 100; j > -100; j -= 100) {
                putImageScale(edge, i, j, 100, 100);
            }
        }
    }
    std::cout << "^:" << clock() - time << " ";
    time = clock();
    if (left + WINDOW_WIDTH >= MAP_WIDTH) {
        for (int i = MAP_WIDTH - left; i < WINDOW_WIDTH; i += 100) {
            for (int j = -up % 100 - 100; j < WINDOW_HEIGHT; j += 100) {
                putImageScale(edge, i, j, 100, 100);
            }
        }
    }
    std::cout << ">:" << clock() - time << " ";
    time = clock();
    if (up + WINDOW_HEIGHT >= MAP_HEIGHT) {
        for (int i = -left % 100 - 100; i < WINDOW_WIDTH; i += 100) {
            for (int j = MAP_HEIGHT - up; j < WINDOW_HEIGHT; j += 100) {
                putImageScale(edge, i, j, 100, 100);
            }
        }
    }
    std::cout << "v:" << clock() - time << " ";
    time = clock();
    for (int i = 0; i < MAP_WIDTH; i+=KERNEL_SIZE) {
        for (int j = 0; j < MAP_HEIGHT; j+=KERNEL_SIZE) {
            map[i / KERNEL_SIZE][j / KERNEL_SIZE].update();
        }
    }
    std::cout << "update:" << clock() - time << " ";
    time = clock();
    for (int i = left; i < left + WINDOW_WIDTH; i += KERNEL_SIZE)
        for (int j = up; j < up + WINDOW_HEIGHT; j += KERNEL_SIZE)
            if (i >= 0 && j >= 0 && i < MAP_WIDTH && j < MAP_HEIGHT) {
                int x = i / KERNEL_SIZE;
                int y = j / KERNEL_SIZE;
                int X = x * KERNEL_SIZE - left;
                int Y = y * KERNEL_SIZE - up;
                map[x][y].paint(X, Y, X + left, Y + up);
            }
    std::cout << "paint:" << clock() - time << " ";
}

void Map::sowingGlobal(float p) {
    for (int i = 0; i < MAP_WIDTH; i += KERNEL_SIZE)
        for (int j = 0; j < MAP_HEIGHT; j += KERNEL_SIZE)
            map[i / KERNEL_SIZE][j / KERNEL_SIZE].sowing(p);
}

void Map::setCenter(float x, float y) {
    centerX = x;
    centerY = y;
}

void Map::refresh() {
    memcpy(sMap, sMap_, sizeof(sMap));
    memset(sMap_, -1, sizeof(sMap_));
}