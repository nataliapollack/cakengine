#pragma once
#include "Events.hpp"

class SolarPanel
{
public:
    void update();

    void draw();
    bool active();

    void StartMinigame(Event& event);

    void StartNewDay(Event& event);

private:
    bool completed;
    bool toggle;
};