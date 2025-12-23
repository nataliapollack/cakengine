#pragma once
#include "System.hpp"

class PlayerSystem : public System
{
public:
    void init();
    void update(float dt);

private:
    float jump_height;
};