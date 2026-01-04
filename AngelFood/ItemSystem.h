#pragma once
#include "System.hpp"
#include "Events.hpp"
#include "raylib.h"

class ItemSystem : public System
{
public:
    void init();

    void TriggerItemPickedUp(Event& event);
    void TriggerItemDroppedOff(Event& event);
private:
};

class CollectingSystem : public System
{
public:
    void init();
    void TriggerItemDroppedOff(Event& event);
};

class SparkSystem : public System
{
public:
    void init();
    void update();
    void draw();

    void TriggerSparkCollected(Event& event);
    void TriggerSparkSpawned(Event& event);
private:
    bool growing;
    int spark_count;
    float speed;
    float growth_mulitplier;
    int frame_counter;
};