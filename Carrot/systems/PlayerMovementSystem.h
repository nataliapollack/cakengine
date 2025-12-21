#pragma once
#include "System.hpp"
#include "Events.hpp"

#include "raylib.h"

class PlayerMovementSystem : public System
{
public:
    void init();
    void update(float dt);

    void BeginDrawCam();
    void EndDrawCam();

    void ChangeBounds(Event& event);
    void ChangeDay(Event& event);
    void ChangeLock(Event& event);

private:
    bool locked;
    int room;
    int day;

    Camera2D camera;
    Vector2 cam_bounds;
    Vector2 player_bounds;
};