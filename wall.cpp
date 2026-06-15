#include "wall.h"
#include <iostream>
#include <Windows.h>

Wall::Wall(int w, int h) : width(w), height(h) {}

void Wall::draw() const
{
    for (int i = 0; i <= width + 1; ++i) {
        COORD c1 = {static_cast<SHORT>(i), 0};
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c1);
        std::cout << '#';
        COORD c2 = {static_cast<SHORT>(i), static_cast<SHORT>(height + 1)};
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c2);
        std::cout << '#';
    }
    for (int j = 1; j <= height; ++j) {
        COORD c1 = {0, static_cast<SHORT>(j)};
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c1);
        std::cout << '#';
        COORD c2 = {static_cast<SHORT>(width + 1), static_cast<SHORT>(j)};
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), c2);
        std::cout << '#';
    }
}

void Wall::update() {}

std::pair<int, int> Wall::getPos() const
{
    return {0, 0};
}

int Wall::getWidth() const
{
    return width;
}

int Wall::getHeight() const
{
    return height;
}
