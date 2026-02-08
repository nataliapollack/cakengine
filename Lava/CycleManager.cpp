#include "CycleManager.h"
#include "raylib.h"

#include "Events.hpp"
#include "Lava.h"

#include "Coordinator.hpp"

extern  Coordinator gCoordinator;
extern int gCurrentDay;

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

        if (!night)
        {
            current_day++;

            Event hints(Events::Time::DAY_BEGIN);
            gCoordinator.SendEvent(hints);
            gCurrentDay++;

            Event audio(Events::Audio::PLAY_MUSIC);
            audio.SetParam(Events::Audio::ASSET, 0);
            gCoordinator.SendEvent(audio);
        }
        else
        {
            Event hints(Events::Time::NIGHT_BEGIN);
            gCoordinator.SendEvent(hints);

            Event audio(Events::Audio::PLAY_MUSIC);
            audio.SetParam(Events::Audio::ASSET, 1);
            gCoordinator.SendEvent(audio);
        }

    }

    if (gCurrentDay >= 3)
    {
        Event hints(Events::Game::WIN);
        gCoordinator.SendEvent(hints);
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