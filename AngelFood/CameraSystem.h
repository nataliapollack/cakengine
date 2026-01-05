#pragma once
#include "System.hpp"
#include "raylib.h"
#include "Events.hpp"

enum CAMERA_STATES
{
    FOLLOW_PLAYER,
    FREEROAM
};

class CameraSystem : public System
{
public:
    void init();
    void update(float dt);

    void BeginCameraMode();
    void EndCameraMode();
    void SetCurrentEn(Event &event);

private:
    bool obtained_entity;
    bool transition;
   // Camera2D camera;
    CAMERA_STATES state;
    Entity current_entity;

    Vector2 old_point;
    Vector2 new_point;
    float old_zoom;
    float new_zoom;
    float timer;
};