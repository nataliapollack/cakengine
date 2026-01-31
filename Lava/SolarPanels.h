#pragma once
#include "Events.hpp"

#include <vector>

#define MAX_PANELS 5

struct tile
{
    int col;
    int row;
};

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

    int placed_panel_count = 0;
    tile current_tiles[MAX_PANELS];

    std::vector<std::vector<bool>> solar_grid;
};