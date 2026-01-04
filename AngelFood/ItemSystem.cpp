#include "ItemSystem.h"
#include "Coordinator.hpp"
#include "Game.h"
#include "Core.h"
#include "Progression.h"
#include "Player.h"
#include "ParticleEmitter.h"
#include "raylib.h"
#include "raymath.h"

#include "AssetManager.h"

#include <iostream>

extern AssetManager gAssetMngr;


extern Coordinator gCoordinator;

void ItemSystem::init()
{
    gCoordinator.AddEventListener(METHOD_LISTENER(Events::Item::CONFIRMED_PICKEDUP, ItemSystem::TriggerItemPickedUp));
}

void ItemSystem::TriggerItemPickedUp(Event& event)
{
    Entity item_id = event.GetParam<Entity>(Events::Item::PickedUp::ITEMID);
    std::cout << item_id << "\n";
    for (auto& entity : entities_list)
    {
        if (!gCoordinator.HasComponent<particle_emitter>(entity))
        {
            auto& coll = gCoordinator.GetComponent<collidble>(entity);

            Vector2 base_offset{ coll.box.width / 2.0f, 
                coll.box.height / 2.0f };

            float offset = 50.0f;

            Vector2 min_offset{ base_offset.x - offset, 
                base_offset.y - offset };
            Vector2 max_offset{ base_offset.x + offset, 
                base_offset.y + offset };

            gCoordinator.AddComponent<particle_emitter>(entity,
                particle_emitter
                {
                    64,         // capacity
                    0,           // alive count
                    { min_offset, max_offset },
                    ColorAlpha(WHITE, 0.7f), // color
                    Vector2Rotate(Vector2Scale(Vector2UnitY, -1.0f), 
                        DEG2RAD * -45.0f), // init dir
                    90.0f,
                    { 400.0f, 800.0f },      // init speed
                    {0.50f, 1.0f},        // init lifetime
                    {5.0f, 10.0f}, // init size
                    16,          // num per emit
                    false ,        // emitting
                    true,       // one shot effect
                    COUNT,
                    Timer(0.0f), // time between emits
                    ET_ITEM_PICKUP,
                    {}
                }
            );
        }

        if (item_id == entity)
        {
            auto& stats = gCoordinator.GetComponent<status>(entity);
            auto& staus = gCoordinator.GetComponent<collectable>(entity);
            staus.picked_up = true;
            stats.active = false;

            auto& emit = gCoordinator.GetComponent<particle_emitter>(entity);
            emit.emitting = true;

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
    gCoordinator.AddEventListener(METHOD_LISTENER(Events::Item::CONFIRMED_DROPPEDOFF, CollectingSystem::TriggerItemDroppedOff));
}

void CollectingSystem::TriggerItemDroppedOff(Event& event)
{
    HOLDABLE_ITEMS item_id = event.GetParam<HOLDABLE_ITEMS>(Events::Item::DroppedOff::OBJECTID);
    //  std::cout << item_id << "\n";
    for (auto& entity : entities_list)
    {
        auto& staus = gCoordinator.GetComponent<collecting>(entity);
        if (staus.item == item_id)
        {
            staus.amount_needed--;
            auto& set = gCoordinator.GetComponent<status>(entity);
            if (staus.amount_needed <= 0)
            {
                set.active = false;
                // spawn spark here :p
                // swap npc state here
            }
            return;
        }
    }
}

void SparkSystem::init()
{
    gCoordinator.AddEventListener(
        METHOD_LISTENER(Events::Collision::SPARK, SparkSystem::TriggerSparkCollected));
}

void SparkSystem::update()
{
    for (auto& entity : entities_list)
    {
        auto& stats = gCoordinator.GetComponent<status>(entity);
        auto& sparky = gCoordinator.GetComponent<spark>(entity);

        if (stats.active)
        {
            if (sparky.frame_counter >= (60 / 5.0f))
            {
                sparky.frame_counter = 0;
                sparky.current_asset = (ASSETS)((int)sparky.current_asset + 1);

                if (sparky.current_asset == TITLE)
                {
                    sparky.current_asset = SPARK1;
                }
            }
            sparky.frame_counter++;
        }
    }
}

void SparkSystem::draw()
{
    for (auto& entity : entities_list)
    {
        auto const& stats = gCoordinator.GetComponent<status>(entity);

        if (stats.active)
        {
            auto const& transform = gCoordinator.GetComponent<transform2D>(entity);
            auto const& rend = gCoordinator.GetComponent<spark>(entity);

            Texture2D texture = gAssetMngr.GetAsset(rend.current_asset);

            Rectangle source = { 0, 0, texture.width, texture.height };
            Vector2 dim = { texture.width * 0.55, texture.height * 0.55};
            Rectangle dest = { transform.pos.x, transform.pos.y, dim.x, dim.y };
            Vector2 origin = { 0, 0 };
            DrawTexturePro(texture, source, dest, origin, 0.0f, WHITE);
        }
    }
}

void SparkSystem::TriggerSparkCollected(Event& event)
{
    spark_count++;

    Entity item_id = event.GetParam<Entity>(Events::Spark::Collected::SPARKID);
    //  std::cout << item_id << "\n";
    for (auto& entity : entities_list)
    {
        auto& staus = gCoordinator.GetComponent<status>(entity);
        if (entity == item_id)
        {
            staus.active = false;
        }
    }
}

void SparkSystem::TriggerSparkSpawned(Event& event)
{

}