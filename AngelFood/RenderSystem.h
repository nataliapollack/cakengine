#pragma once
#include "raylib.h"
#include "System.hpp"

#include <vector>

class RenderSystem : public System
{
public:
    void init();
    void draw();
    void ReorganizeObjects();

private:
    std::vector<std::uint32_t> draw_order;
};