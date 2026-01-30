#pragma once
#include "System.hpp"
#include "Events.hpp"
#include "raylib.h"

class DoorSystem : public System
{
public:
    void init();

    void ChangeRoom(Event& event);

private:
    Vector2 player_outside_pos;
    Vector2 player_inside_pos;
};