#pragma once
#include "raylib.h"
#include "System.hpp"
#include "Events.hpp"
#include "Game.h"

class ObstacleSystem : public System
{
public:
    void init();
    void update(float dt);

    void TriggerObstacleHit(Event &event);

    void display_obj_count_debug();
    void reset();

private:
    bool continue_updating;
    float obstalce_speed = 5.0f;
    int obstacle_count;
};