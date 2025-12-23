#include "ItemSystem.h"
#include "Coordinator.hpp"
#include "Game.h"
#include "Core.h"
#include "Progression.h"

extern Coordinator gCoordinator;

void ItemSystem::init()
{
    gCoordinator.AddEventListener(METHOD_LISTENER(Events::Item::PICKED_UP, ItemSystem::TriggerItemPickedUp));
}

void ItemSystem::TriggerItemPickedUp(Event& event)
{
    int item_id = event.GetParam<int>(Events::Item::OBJ_TYPE);

    for (auto& entity : entities_list)
    {
        if (item_id == entity)
        {
            auto& stats = gCoordinator.GetComponent<status>(entity);
            stats.active = false;
            return;
        }
    }
}

void ItemSystem::TriggerQuestComplete(Event& event)
{
    int item_id = event.GetParam<int>(Events::Item::OBJ_TYPE);

    for (auto& entity : entities_list)
    {
        auto& stats = gCoordinator.GetComponent<status>(entity);

        if (item_id == entity)
        {
            gCoordinator.DestroyEntity(entity);
        }

        if (stats.type == DROPOFF)
        {
            auto& drop_off = gCoordinator.GetComponent<collecting>(entity);
            if (drop_off.item_id == item_id)
            {
                gCoordinator.DestroyEntity(entity);
            }
        }
    }
}