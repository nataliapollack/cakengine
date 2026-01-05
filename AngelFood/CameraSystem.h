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
    void update();

    void BeginCameraMode();
    void EndCameraMode();
    void SetCurrentEn(Event &event);

private:
    bool obtained_entity;
   // Camera2D camera;
    CAMERA_STATES state;
    Entity current_entity;
};