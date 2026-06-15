#include "food.h"
#include "snake.h"
#include <iostream>

Food::Food() : pos{0, 0} {}

Food::Food(std::pair<int, int> position) : pos(position) {}

Food::Food(int width, int height, const SnakeEntity& snake)
{
    for (int attempt = 0; attempt < 200; attempt++) {
        int fx = rand() % width + 1;
        int fy = rand() % height + 1;
        if (!snake.checkCollision(fx, fy)) {
            pos = {fx, fy};
            return;
        }
    }
    for (int x = 1; x <= width; ++x) {
        for (int y = 1; y <= height; ++y) {
            if (!snake.checkCollision(x, y)) {
                pos = {x, y};
                return;
            }
        }
    }
    pos = {0, 0};
}

void Food::draw() const
{
    std::cout << 'F';
}

void Food::update() {}

std::pair<int, int> Food::getPos() const
{
    return pos;
}

void Food::setPos(std::pair<int, int> p)
{
    pos = p;
}
