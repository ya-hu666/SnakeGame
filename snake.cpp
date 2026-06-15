#include "snake.h"
#include <Windows.h>
#include <iostream>

Snake::Snake(int width, int height)
{
    body.push_back({ width / 2, height / 2 });
    body.push_back({ width / 2 - 1, height / 2 });
    body.push_back({ width / 2 - 2, height / 2 });
}

Snake::Snake(int width, int height, int x, int y)
{
    for (int i = 0; i < 3; ++i)
        body.push_back({ x - i, y });
}

Snake::Snake(int width, int height, int x, int y, int initLength)
{
    for (int i = 0; i < initLength; ++i)
        body.push_back({ x - i, y });
}

void Snake::draw() const
{
    if (body.empty()) return;
    COORD c = { static_cast<SHORT>(body.front().first),
                static_cast<SHORT>(body.front().second) };
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c);
    std::cout << "0";
}

void Snake::update()
{
}

std::pair<int, int> Snake::getPos() const
{
    return body.empty() ? std::pair<int, int>{0, 0} : body.front();
}

void Snake::pushHead(std::pair<int, int> newHead)
{
    body.push_front(newHead);
}

void Snake::popTail()
{
    prevTail = body.back();
    body.pop_back();
}

int Snake::getLength() const
{
    return static_cast<int>(body.size());
}

bool Snake::isOccupying(int x, int y) const
{
    for (auto& segment : body)
        if (x == segment.first && y == segment.second)
            return true;
    return false;
}

std::pair<int, int> Snake::showHead() const
{
    return body.front();
}

std::pair<int, int> Snake::getTail() const
{
    return body.back();
}

Snake::Direction Snake::getDir() const
{
    return dir;
}

void Snake::setDir(Direction d)
{
    dir = d;
}

bool Snake::logic(int width, int height, int& score)
{
    std::pair<int, int> newHead = body.front();
    switch (dir) {
    case UP:    newHead.second--; break;
    case DOWN:  newHead.second++; break;
    case LEFT:  newHead.first--; break;
    case RIGHT: newHead.first++; break;
    default: break;
    }
    if (newHead.first <= 0 || newHead.first > width ||
        newHead.second <= 0 || newHead.second > height)
        return false;
    for (auto& segment : body)
        if (segment == newHead)
            return false;
    body.push_front(newHead);
    return true;
}

SnakeEntity::SnakeEntity(int width, int height)
    : Snake(width, height) {}

SnakeEntity::SnakeEntity(int width, int height, int x, int y)
    : Snake(width, height, x, y) {}

SnakeEntity::SnakeEntity(int width, int height, int x, int y, int initLength)
    : Snake(width, height, x, y, initLength) {}

bool SnakeEntity::checkCollision(int x, int y) const
{
    return isOccupying(x, y);
}
