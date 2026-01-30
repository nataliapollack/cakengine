#pragma once
#include "System.hpp"
#include "raylib.h"

#include <vector>
#include <cstdint>

class RenderSystem : public System
{
public:
    void init();
    void shutdown();

    void render_viewports();
    void draw();

private:
    void ReorganizeObjects();
    std::vector<std::uint32_t> draw_order;
};