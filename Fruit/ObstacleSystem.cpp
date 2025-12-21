#include "ObstacleSystem.h"

#include "Coordinator.hpp"
#include "raylib.h"
#include "Core.h"
#include "Game.h"

#define FLOOR 700

extern Coordinator gCoordinator;

void ObstacleSystem::init()
{
    gCoordinator.AddEventListener(METHOD_LISTENER(Events::Obstacles::OBST, ObstacleSystem::TriggerObstacleHit));

    obstalce_speed = 5.0f;
    obstacle_count = 0;
    continue_updating = true;
}

void ObstacleSystem::update(float dt)
{
    if (obstacle_count >= 25 && continue_updating)
    {
        Event coll_door(Events::Race::FINISHED);

        gCoordinator.SendEvent(coll_door);
        continue_updating = false;
    }
    
    for (auto& entity : entities_list)
    {
        auto& transf = gCoordinator.GetComponent<transform2>(entity);
        if (transf.pos.y >= FLOOR && obstacle_count < 150)
        {
            transf.pos.y = GetRandomValue(-100, -1000);
            transf.pos.x = GetRandomValue(100, GetScreenWidth()- 100);

            obstacle_count++;
        }

        transf.pos.y += obstalce_speed * dt;
    }

    obstalce_speed += 1.0f;
}

void ObstacleSystem::display_obj_count_debug()
{
    DrawText(TextFormat("count %d", obstacle_count), 10, 10, 20, BLACK);
}

void ObstacleSystem::TriggerObstacleHit(Event &event)
{
    obstalce_speed = 5.0f;
}

void ObstacleSystem::reset()
{
    obstacle_count = 0;
    continue_updating = true;
}