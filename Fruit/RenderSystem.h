#pragma once
#include "raylib.h"
#include "System.hpp"
#include <vector>

class RenderSystem : public System
{
public:
    void init();
    void ReorganizeObjects();
    void draw();

private:
    std::vector<std::uint32_t> draw_order;
};