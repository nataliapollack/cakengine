#include "Events.hpp"

#include <vector>
#include "raylib.h"
#include "raymath.h"

#define WAVE_POINTS 246


class AudioFreq
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
    bool begin_timer;

    Vector2 Correct_sinePoints[WAVE_POINTS];
    Vector2 Current_sinePoints[WAVE_POINTS];
    float Current_angle = 0.0f;
    float Correct_angle;

    Vector2 knobs_pos[2];
    float radius;

    float timer;
};