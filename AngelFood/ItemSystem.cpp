#include "ItemSystem.h"
#include "Coordinator.hpp"
#include "Game.h"
#include "Core.h"
#include "Progression.h"
#include "Player.h"

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
            auto& staus = gCoordinator.GetComponent<collectable>(entity);
            staus.picked_up = true;
            stats.active = false;
            //gCoordinator.RemoveComponent<render>(entity);
            return;
           // gCoordinator.DestroyEntity(entity);
        }
    }
}

void ItemSystem::TriggerItemDroppedOff(Event& event)
{
    Entity item_id = event.GetParam<OBJECT_TYPE>(Events::Item::DroppedOff::ITEMID);
    for (auto& entity : entities_list)
    {
        auto& staus = gCoordinator.GetComponent<status>(entity);
        if (staus.type == item_id)
        {
            auto& set = gCoordinator.GetComponent<status>(entity);
            set.active = false;
        }
    }
}

void CollectingSystem::init()
{
    gCoordinator.AddEventListener(METHOD_LISTENER(Events::Item::DROPPEDOFF, CollectingSystem::TriggerItemDroppedOff));
}

void CollectingSystem::TriggerItemDroppedOff(Event& event)
{
    Entity item_id = event.GetParam<HOLDABLE_ITEMS>(Events::Item::DroppedOff::ITEMID);
    //  std::cout << item_id << "\n";
    for (auto& entity : entities_list)
    {
        auto& staus = gCoordinator.GetComponent<collecting>(entity);
        if (staus.item == item_id)
        {
            
            auto& set = gCoordinator.GetComponent<status>(entity);
            set.active = false;

            return;
        }
    }
}