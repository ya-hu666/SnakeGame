#pragma once
#include "entity.h"
#include <cstdlib>

class SnakeEntity;

class Food : public virtual Entity {
    std::pair<int, int> pos;
public:
    Food();
    explicit Food(std::pair<int, int> position);
    Food(int width, int height, const SnakeEntity& snake);

    void draw() const override;
    void update() override;
    std::pair<int, int> getPos() const override;

    void setPos(std::pair<int, int> p);
};
