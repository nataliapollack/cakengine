#pragma once
#include "System.hpp"
#include "Events.hpp"
#include "raylib.h"

class ItemSystem : public System
{
public:
    void init();

    void TriggerItemPickedUp(Event& event);
    void TriggerQuestComplete(Event& event);
private:
};