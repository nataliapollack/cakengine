#pragma once
#include "raylib.h"

#include "Events.hpp"

class TransitionManager
{
public:
    void init();
    void init_in();
    void init_out();

    void update();
    void update_fade_in();

    void update_fade_out();
    void draw_fade();

    void update_fade_white();

    void TriggerFadeIn(Event& event);
    void TriggerFadeOut(Event& event);
    void TriggerDay(Event& event);
    void TriggerWhite(Event& event);

    bool GetGanmeOver();

private:
    bool game_done;
    bool fade_in; 
    bool fade_out;
    bool fade_white;
    bool new_day;

    int current_frame = 0;
    float fade_timer = 5.0f;
    float current_fade = 0.0f;
};