#pragma once
#include "entity.h"
#include <vector>

class Wall : public virtual Entity {
    int width;
    int height;
public:
    Wall(int w, int h);

    void draw() const override;
    void update() override;
    std::pair<int, int> getPos() const override;

    int getWidth() const;
    int getHeight() const;
};
