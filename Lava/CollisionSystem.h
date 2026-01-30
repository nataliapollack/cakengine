#pragma once
#include "System.hpp"

class CollisionSystem : public System
{
public:
    void CheckCollisions();

    void DebugDrawBoxes();
private:
    bool toggle;
};