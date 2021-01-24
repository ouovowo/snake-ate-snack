#pragma once
#include "acllib.h"
#include "snake_map.h"
#include <vector>
#include <deque>
#include <cmath>

#define MAX(x, y) (((x)>(y))?(x):(y))
#define MIN(x, y) (((x)<(y))?(x):(y))

const int INITIAL_LENGTH = 10;

struct Node {
    float x, y, deg;
};

typedef std::deque<Node> Path;
typedef std::vector<Node> Body;
typedef std::vector<ACL_Image *> Skin;

class Snake {
protected:
    int id;
    Skin skin;
    Path path;
    Body body;
    float r;
    float deg;
    float v, w;
    int score;
    int dscore;
    int length;
    static Map *pmap;
    ACL_Image *destroyed;
    void transfer2map(int id);
    void rotateImage(Node node, float r);
    bool dangerous(float x, float y);
    static int counter;
    static Skin head;
public:
    static void loadHead();
    virtual ~Snake();
    Snake() {}
    Snake(Snake &) {}
    Snake(float x, float y, float r, Skin skin, ACL_Image *des);
    void add();
    void eat();
    bool dead();
    void paint();
    void destroy();
    void move(float dt);
    void rotate(float dt);
    int getW() { return w; }
    void setW(float W) { w = W; }
    void setV(float V) { v = V; }
    std::pair<int, int> getHead();
    static void setMap(Map *map);
    virtual void setCenter() {}
    virtual void navigate() {}
};