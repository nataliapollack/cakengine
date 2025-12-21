#include "TimerManager.h"

#include "Coordinator.hpp"
#include "Game.h"

extern Coordinator gCoordinator;

void SkiTimer::init()
{
    cooldown = 3.0f;
    timer = 0.0f;
    frame_counter = 0;
    points = 0;
    gCoordinator.AddEventListener(METHOD_LISTENER(Events::Obstacles::FLAGS, SkiTimer::AddPoints));
}

void SkiTimer::update()
{
    frame_counter++;
    if (frame_counter >= 60)
    {
        timer += 1.0f;
        frame_counter = 0.0f;
        cooldown -= 1.0f;
    }
}

void SkiTimer::reset_timer()
{
    timer = 0.0f;
    points = 0;
}

void SkiTimer::draw()
{
    DrawText(TextFormat("time %f", timer), 10, 50, 20, BLACK);
}

void SkiTimer::AddPoints(Event& event)
{
    if (cooldown <= 0.0f)
    {
        cooldown = 3.0f;
        points += 100;
    }
}

float SkiTimer::GetPoints()
{
    return points - (timer * 2);
}