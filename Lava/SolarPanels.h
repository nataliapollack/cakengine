#pragma once
#include "Events.hpp"
#include "raylib.h"
#include <vector>

#define MAX_PANELS 5

struct Panel
{
    Vector2 pos;
    bool correct;
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

    void ShowHint(Event& event);

    void CheckCorrectness(float x, float y);

private:
    bool completed;
    bool toggle;
    bool show_warning;
    bool show_hints;

    int placed_panel_count = 0;
    float timer;

    std::vector<std::vector<bool>> solar_grid;
    Panel correct_panels[10];
};