
#include "DoorSystem.h"
#include "Coordinator.hpp"

#include "Core.h"

extern Coordinator gCoordinator;

void DoorSystem::init()
{

    gCoordinator.AddEventListener(
        METHOD_LISTENER(Events::Collision::HIT_DOOR, DoorSystem::ChangeRoom));
}

void DoorSystem::ChangeRoom(Event& event)
{
    int room = event.GetParam<int>(Events::Collision::COLLISION_DATA);

    for (auto& entity : entities_list)
    {
        auto& stats = gCoordinator.GetComponent<status>(entity);

        if (stats.room != room)
        {
            stats.active = false;
        }

        if (stats.type == PLAYER)
        {
            auto& transform = gCoordinator.GetComponent<transform2D>(entity);

            // 1 is inside , 2 is outside -> see screenmanager.h
            transform.pos = (room == 1) ? player_inside_pos : player_outside_pos;
        }
    }

    // now send for screenchange
    Event new_room(Events::Game::SCREEN_CHANGE);

    new_room.SetParam(Events::Game::SCREEN_ID,
        room);
}