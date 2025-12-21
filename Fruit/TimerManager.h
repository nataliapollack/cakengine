#pragma once
#include "raylib.h"
#include "Events.hpp"

class SkiTimer
{
public:
    void init();
    void update();

    void reset_timer();
    void draw();

    void AddPoints(Event& event);

    float GetPoints();
private:
    int points;
    float timer;
    float cooldown;

    int frame_counter;
};