#pragma once
#include "System.hpp"

class CollisionSystem : public System
{
public:
    void CheckCollisions();

    void debug_draw_collisions();

private:
};