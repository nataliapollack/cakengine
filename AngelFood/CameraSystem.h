#pragma once
#include "System.hpp"
#include "raylib.h"

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

private:
    Camera2D camera;
    CAMERA_STATES state;
};