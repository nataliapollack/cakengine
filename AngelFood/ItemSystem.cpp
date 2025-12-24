#include "ItemSystem.h"
#include "Coordinator.hpp"
#include "Game.h"
#include "Core.h"
#include "Progression.h"

#include <iostream>

extern Coordinator gCoordinator;

void ItemSystem::init()
{
    gCoordinator.AddEventListener(METHOD_LISTENER(Events::Item::PICKEDUP, ItemSystem::TriggerItemPickedUp));
}

void ItemSystem::TriggerItemPickedUp(Event& event)
{
    Entity item_id = event.GetParam<Entity>(Events::Item::PickedUp::ITEMID);
  //  std::cout << item_id << "\n";
    for (auto& entity : entities_list)
    {
        if (item_id == entity)
        {
            auto& stats = gCoordinator.GetComponent<status>(entity);
            stats.active = false;
            gCoordinator.RemoveComponent<render>(entity);
            return;
           // gCoordinator.DestroyEntity(entity);
        }
    }
}