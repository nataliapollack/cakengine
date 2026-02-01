#pragma once
#include "Events.hpp"

#include <vector>

#define MAX_PANELS 5

class SolarPanel
{
public:
    void init();

    void update();

    void draw();

    bool active();

    void StartMinigame(Event& event);

    void StartNewDay(Event& event);
   

private:
    bool completed;
    bool toggle;
    bool show_warning;

    int placed_panel_count = 0;
    float timer;

    std::vector<std::vector<bool>> solar_grid;
};