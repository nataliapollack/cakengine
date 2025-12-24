#pragma once
#include "raylib.h"
#include "System.hpp"
#include "Game.h"

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

class BoxRenderSystem : public System
{
public:
    void init();
    void draw();
    void ReorganizeObjects();
private:
    std::vector<std::uint32_t> draw_order;
};