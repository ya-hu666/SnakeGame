#pragma once
#include <utility>

class Entity {
public:
    virtual ~Entity() = default;

    virtual void draw() const = 0;
    virtual void update() = 0;
    virtual std::pair<int, int> getPos() const = 0;
};
