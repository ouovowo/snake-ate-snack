#include "snake_sprite.h"
#include "snake_user.h"
#include "snake_auto.h"
#include "snake_map.h"
#include "acllib.h"
#include <iostream>
#include <vector>
#include <ctime>
#include <list>

time_t DieTime;
bool GameOver = false;

ACL_Image blue, cyan, jade, head, back, edge, des;
const int N_SKIN = 3;
Skin skin[N_SKIN];
Skin skin_;

Map map;
Snake *snake_user;
const int MAXN_SNAKE = 20;
std::list<Snake *> snake_auto;

void paint();
void update(int id);
void kbEvent(int key, int event);
void createSnakeAuto();
void loadResource();

int Setup() {
    srand(time(0));
    initWindow("S8S: Snake Ate Snack", 33, 0, WINDOW_WIDTH, WINDOW_HEIGHT);

    loadResource();
    Snake::setMap(&map);
    snake_user = new snakeUser(MAP_WIDTH / 2, MAP_HEIGHT / 2, 20, skin_, &des);

    registerTimerEvent(update);
    registerKeyboardEvent(kbEvent);

    startTimer(0, 15);
    startTimer(1, 1000);

    return 0;
}

void loadResource() {
    loadImage("python.gif", &des);
    loadImage("head.gif", &head);
    loadImage("blue.gif", &blue);
    loadImage("cyan.gif", &cyan);
    loadImage("jade.gif", &jade);
    loadImage("back.gif", &back);
    loadImage("edge.gif", &edge);
    map.setTexture(&back, &edge);

    skin_.push_back(&head);
    skin_.push_back(&blue);
    skin_.push_back(&cyan);
    skin_.push_back(&jade);

    skin[0].push_back(&head);
    skin[0].push_back(&blue);

    skin[1].push_back(&head);
    skin[1].push_back(&cyan);

    skin[2].push_back(&head);
    skin[2].push_back(&jade);

    Snake::loadHead();
}

void update(int id){
    switch (id) {
    case 0:
        paint();
        break;
    case 1:
        map.sowingGlobal(0.003);
    }
}

void paint() {
    beginPaint();
    clearDevice();

    clock_t ctime = clock();
    if (!GameOver) {
        snake_user->navigate();
        snake_user->rotate(1);
        snake_user->move(1);
    }
    else {
        if (time(0)-DieTime==3) {
            snake_user = new snakeUser(map.getX(), map.getY(), 20, skin_, &des);
            GameOver = false;
        }
    }
    std::cout << "userMove:" << clock() - ctime << " ";

    ctime = clock();
    for (auto it = snake_auto.begin(); it != snake_auto.end(); it++) {
        (*it)->navigate();
        (*it)->rotate(1);
        (*it)->move(1);
    }
    std::cout << "autoMove:" << clock() - ctime << " ";

    ctime = clock();
    map.display();
    std::cout << "backShow:" << clock() - ctime << " ";

    ctime = clock();
    for (auto it = snake_auto.begin(); it != snake_auto.end(); ) {
        if ((*it)->dead()) {
            delete (*it);
            snake_auto.erase(it++);
        }
        else {
            (*it)->paint();
            it++;
        }
    }
    std::cout<< "autoShow:" << clock() - ctime << " ";

    ctime = clock();
    if (!GameOver) {
        if (snake_user->dead()) {
            delete snake_user;
            GameOver = true;
            DieTime = time(0);
        }
        else {
            snake_user->paint();
        }
    }
    std::cout<< "userShow:" << clock() - ctime << "\n";

    if (snake_auto.size() < MAXN_SNAKE && float(rand())/RAND_MAX < 0.05) {
        createSnakeAuto();
    }
    map.refresh();
    endPaint();
}

void kbEvent(int key, int event) {
    if (!GameOver) {
        if (event == KEY_UP) {
            snake_user->setW(0);
        }
        if (event == KEY_DOWN) {
            if (key == VK_LEFT) {
                snake_user->setW(5);
            }
            if (key == VK_RIGHT) {
                snake_user->setW(-5);
            }
            if (key == VK_SPACE) {
                snake_user->add();
            }
        }
    }
}

void createSnakeAuto() {
    int x = rand() % MAP_WIDTH;
    int y = rand() % MAP_HEIGHT;
    snake_auto.push_back(new snakeAuto(x, y, 20, skin[rand() % N_SKIN], &des));
}