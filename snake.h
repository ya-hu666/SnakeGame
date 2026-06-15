#pragma once
#include "entity.h"
#include <deque>

class Collidable : public virtual Entity {
public:
    virtual bool checkCollision(int x, int y) const = 0;
};

class Snake : public virtual Entity {
public:
    enum Direction { UP = 1, RIGHT = 2, DOWN = 3, LEFT = 4 };

    std::pair<int, int> prevTail{0, 0};
private:
    std::deque<std::pair<int, int>> body;
    Direction dir = RIGHT;
public:

    Snake(int width, int height);
    Snake(int width, int height, int x, int y);
    Snake(int width, int height, int x, int y, int initLength);

    void draw() const override;
    void update() override;
    std::pair<int, int> getPos() const override;

    void pushHead(std::pair<int, int> pos);
    void popTail();
    int getLength() const;
    bool isOccupying(int x, int y) const;
    std::pair<int, int> showHead() const;
    std::pair<int, int> getTail() const;
    Direction getDir() const;
    void setDir(Direction d);
    bool logic(int width, int height, int& score);
};

class SnakeEntity : public Snake, public Collidable {
public:
    SnakeEntity(int width, int height);
    SnakeEntity(int width, int height, int x, int y);
    SnakeEntity(int width, int height, int x, int y, int initLength);

    bool checkCollision(int x, int y) const override;
};
