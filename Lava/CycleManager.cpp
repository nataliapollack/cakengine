#include "CycleManager.h"
#include "raylib.h"

void CycleManager::init()
{
    night = false;
    current_time = MAX_DAY_TIME;
}

// idk if anything needs to know when it's night or not lol
void CycleManager::UpdateTimer(float dt)
{
    current_time -= dt;

    if (current_time <= 0)
    {
        night = !night;

        current_time = night ? MAX_NIGHT_TIME : MAX_DAY_TIME;

        if (!night) current_day++;

    }
}

// later will use this to also lock the player out from interacting w/ certain objs (mainly just prevenitng htem from exiting / audio freq puzzle / maze)
bool CycleManager::NightOrDay()
{
    return night;
}

void CycleManager::draw()
{
    // temp. we might have an image here instead vs a countdown but whatever
    Color col = BLUE;
    if (night)
    {
        col = PINK;
        DrawRectangle(0, 0, 900, 600, ColorAlpha(BLACK, 0.5));
    }

    DrawText(TextFormat("debug timer: %i", (int)current_time), 700, 10, 20, col);
    // also i might change this based on how i feel about it.
}