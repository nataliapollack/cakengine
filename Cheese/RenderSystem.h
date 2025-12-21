#pragma once
#include "System.hpp"
#include "raylib.h"

#include <vector>
#include <cstdint>

class RenderSystem : public System
{
public:
    void init();
  //  void update(float dt);
    void update();
    void draw();
    void ReorganizeObjects();

private:
    std::vector<std::uint32_t> draw_order;
};