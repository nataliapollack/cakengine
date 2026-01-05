#pragma once
#include "System.hpp"
#include "Events.hpp"
#include "raylib.h"
#include "Timer.hpp"

class ItemSystem : public System
{
public:
    void init();

    void TriggerItemPickedUp(Event& event);
    void TriggerItemDropped(Event& event);
private:

};

// the sheep and da king bird
class CollectingSystem : public System
{
public:
    void init();
    void TriggerItemDroppedOff(Event& event);
private:
    Vector2 positions[4];
    int position_counter;
};

class SparkSystem : public System
{
public:
    void init();
    void update(float dt);
    void draw();

    void TriggerSparkCollected(Event& event);
    void TriggerSparkSpawned(Event& event);
private:
    bool growing;
    int spark_count;
    float speed;
    float growth_mulitplier;
    int frame_counter;

    Timer spawn_time;
    bool finished_spawn;
};


// this is literally just the snake.. idc...
class WaitingGameSystem : public System
{
public:
    void init();
    void update();
    void TriggerStartGame(Event& event);

private:
    bool playing;
    bool just_swapped;
    bool done;
    float time_spent;
    float max_time_spent;
    float delay_timer;
    int frame_counter;
};