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
    gCoordinator.AddEventListener(METHOD_LISTENER(Events::Item::DROPPED_SPIKES, ItemSystem::TriggerItemDropped));
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

void ItemSystem::TriggerItemDropped(Event& event)
{
    for (auto& entity : entities_list)
    {
        auto& stats = gCoordinator.GetComponent<status>(entity);
        auto& staus = gCoordinator.GetComponent<collectable>(entity);
        if (!stats.active && staus.picked_up)
        {
            staus.picked_up = false;
            stats.active = true;
        }
    }
}

void CollectingSystem::init()
{
    gCoordinator.AddEventListener(METHOD_LISTENER(Events::Item::CONFIRMED_DROPPEDOFF, CollectingSystem::TriggerItemDroppedOff));

    position_counter = 0;
    positions[0] = Vector2{ -5450, 410 };
    positions[1] = Vector2{ -5350, 410 };
    positions[2] = Vector2{ -5250, 410 };
    positions[3] = Vector2{ -6350, -2540 };
}

void CollectingSystem::TriggerItemDroppedOff(Event& event)
{
    HOLDABLE_ITEMS item_id = event.GetParam<HOLDABLE_ITEMS>(Events::Item::DroppedOff::OBJECTID);
    int amount = event.GetParam<int>(Events::Item::DroppedOff::AMOUNT);

    //  std::cout << item_id << "\n";
    for (auto& entity : entities_list)
    {
        auto& staus = gCoordinator.GetComponent<collecting>(entity);
        if (staus.item == item_id)
        {
            staus.amount_needed -= amount;
            auto& set = gCoordinator.GetComponent<status>(entity);
            auto& tranform = gCoordinator.GetComponent<transform2D>(entity);

            if (staus.amount_needed <= 0)
            {
               //a set.active = false;
                // spawn spark here :p
                
                // swap npc state here.. unless... we're the npc state ...
                Event spark(Events::Spark::SPAWN);
                spark.SetParam(Events::Spark::Collected::LOCATIONX, tranform.pos.x);
                spark.SetParam(Events::Spark::Collected::LOCATIONY, tranform.pos.y);
                gCoordinator.SendEvent(spark);

                auto& rend = gCoordinator.GetComponent<render_environment>(entity);
                auto& anime = gCoordinator.GetComponent<animate>(entity);
                if (staus.item == BIRD)
                {
                    rend.txt = KING_HAPPY1;
                    anime.alt_asset = KING_HAPPY2;
                }
                if (staus.item == FRUIT)
                {
                    rend.txt = SHEEP_HAPPY1;
                    anime.alt_asset = SHEEP_HAPPY2;
                }
                
            }

            if (staus.item == FRUIT)
            {
                int dropped_ec = gCoordinator.CreateEntity();
                gCoordinator.AddComponent(dropped_ec, transform2D{ Vector2{positions[position_counter].x, positions[position_counter].y} });
                position_counter++;

                gCoordinator.AddComponent(dropped_ec, render_environment{ true, false, false, FRUIT1, 1, 0.75, 0.0f });
                gCoordinator.AddComponent(dropped_ec, status{ true, true,  ENVIRONMENT });
                gCoordinator.AddComponent(dropped_ec, animate{ 5.0f,  0,  FRUIT2 });
            }
            else
            {
                int dropped_ec = gCoordinator.CreateEntity();
                gCoordinator.AddComponent(dropped_ec, transform2D{ positions[3] });
                position_counter++;

                gCoordinator.AddComponent(dropped_ec, render_environment{ true, false, false, BIRD_HAPPY1, 1, 0.75, 0.0f });
                gCoordinator.AddComponent(dropped_ec, status{ true, true,  ENVIRONMENT });
                gCoordinator.AddComponent(dropped_ec, animate{ 5.0f,  0,  BIRD_HAPPY2 });
            }


            return;
        }
    }
}

void SparkSystem::init()
{
    growing = false;

    spawn_time = Timer(3.0f);

    gCoordinator.AddEventListener(
        METHOD_LISTENER(Events::Collision::SPARK, SparkSystem::TriggerSparkCollected));

    gCoordinator.AddEventListener(
        METHOD_LISTENER(Events::Spark::SPAWN, SparkSystem::TriggerSparkSpawned));
}

void SparkSystem::update(float dt)
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

            if (growing)
            {
                growth_mulitplier += 0.015f;
                
                if (growth_mulitplier >= 1.0f)
                {
                    growth_mulitplier = 1.0f;
                    growing = false;
                }
            }
            else
            {
                if (spawn_time.update(dt))
                {
                    finished_spawn = true;
                }
            }
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
            
            Rectangle source = { 0, 0, (float)texture.width, (float)texture.height };
            Vector2 dim = { texture.width * 0.55f, texture.height * 0.55f };
            if (growing)
            {
                dim = { texture.width * 0.55f * growth_mulitplier, texture.height * 0.55f * growth_mulitplier };
            }
            Rectangle dest = { transform.pos.x, transform.pos.y, dim.x, dim.y };
            Vector2 origin = { 0, 0 };
            DrawTexturePro(texture, source, dest, origin, 0.0f, WHITE);
        }
    }
}

void SparkSystem::TriggerSparkCollected(Event& event)
{
    if (!growing)
    {
        spark_count++;

        Entity item_id = event.GetParam<Entity>(Events::Spark::Collected::SPARKID);
        //  std::cout << item_id << "\n";
        for (auto& entity : entities_list)
        {
            auto& staus = gCoordinator.GetComponent<status>(entity);
            if (finished_spawn && entity == item_id)
            {
                staus.active = false;

                auto& coll = gCoordinator.GetComponent<collidble>(entity);

                Vector2 base_offset{ coll.box.width * 0.25f ,
                    coll.box.height * 0.25f };

                float offset = 5.0f;

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
                        ColorAlpha({ 248, 196, 129 }, 0.7f), // color
                        Vector2Rotate(Vector2Scale(Vector2UnitY, -1.0f),
                            DEG2RAD * -45.0f), // init dir
                        360.0f,
                        { 400.0f, 800.0f },      // init speed
                        { 0.05f, 0.25f },        // init lifetime
                        { 2.0f, 6.0f }, // init size
                        64,          // num per emit
                        true ,        // emitting
                        true,       // one shot effect
                        COUNT,
                        Timer(0.0f), // time between emits
                        ET_SPARK_PICKUP,
                        {}
                    }
                );
            }
        }
    }
}

void SparkSystem::TriggerSparkSpawned(Event& event)
{

    int new_spark = gCoordinator.CreateEntity();
    growing = true;
    growth_mulitplier = 0.0f;

    finished_spawn = false;
    spawn_time.start();

    float y = event.GetParam<float>(Events::Spark::Collected::LOCATIONY) - 20;
    float x = event.GetParam<float>(Events::Spark::Collected::LOCATIONX);

    gCoordinator.AddComponent(new_spark, transform2D{ Vector2{x, y} });
    gCoordinator.AddComponent(new_spark, spark{ 0, SPARK1 });
    gCoordinator.AddComponent(new_spark, status{ true, true,  SPARK });
    Texture temp = gAssetMngr.GetAsset(SPARK1);
    gCoordinator.AddComponent(new_spark, collidble{ Rectangle { x, y, (float)temp.width, (float)temp.height } });

    auto const& stats = gCoordinator.GetComponent<status>(new_spark);
}

void WaitingGameSystem::init()
{
    time_spent = 0.0f;
    max_time_spent = 7.0f;
    frame_counter = 0;
    playing = false;
    just_swapped = false;
    delay_timer = 2.0f;
    done = false;

    gCoordinator.AddEventListener(METHOD_LISTENER(Events::Collision::WAITINGGAME, WaitingGameSystem::TriggerStartGame));
}

void WaitingGameSystem::update()
{
    for (auto& entity : entities_list)
    {
        if (!done)
        {
            // swap to idle
            if (just_swapped)
            {
                delay_timer -= (1 / 60.0f);
            }
            if (delay_timer <= 0.0f)
            {
                just_swapped = false;
            }

            if (max_time_spent <= time_spent)
            {
                done = true;
                auto& tranform = gCoordinator.GetComponent<transform2D>(entity);
                {
                    Event spark(Events::Spark::SPAWN);
                    spark.SetParam(Events::Spark::Collected::LOCATIONX, tranform.pos.x);
                    spark.SetParam(Events::Spark::Collected::LOCATIONY, tranform.pos.y);
                    gCoordinator.SendEvent(spark);
                }
            }

            if (!just_swapped && !done && !playing)
            {
                auto& rend = gCoordinator.GetComponent<render_environment>(entity);
                auto& anime = gCoordinator.GetComponent<animate>(entity);
                if (rend.txt != SNAKE_IDLE1 && rend.txt != SNAKE_IDLE2)
                {
                    anime.speed = 3.0f;
                    rend.txt = SNAKE_IDLE1;
                    anime.alt_asset = SNAKE_IDLE2;
                }
            }
            playing = false;
        }
    }
}

void WaitingGameSystem::TriggerStartGame(Event &event)
{
    playing = true;
    if (frame_counter >= 60 )
    {  
        frame_counter = 0;
        time_spent++;
    }
    frame_counter++;

    for (auto& entity : entities_list)
    {
        auto& rend = gCoordinator.GetComponent<render_environment>(entity);
        auto& anime = gCoordinator.GetComponent<animate>(entity);
        if (rend.txt != SNAKE_PLAY1 && rend.txt != SNAKE_PLAY2)
        {
            rend.txt = SNAKE_PLAY1;
            anime.alt_asset = SNAKE_PLAY2;
        }
    }
    just_swapped = true;
}
