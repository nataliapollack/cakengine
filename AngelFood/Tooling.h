#pragma once
#include "System.hpp"
#include "raylib.h"


enum COMPONENTS_LIST
{
    TRANSFORM,
    RENDER,
    BOX_RENDER,
    COLLIDBLE,
    PHYSICS,
    STATUS,
    PLAYERc,
    COLLECTABLE,
    COLLECTING,
    ENVIRONMENT_RENDER
};

class Tooling : public System
{
public:
    void serialize();
    void deserialize();

    bool GetToolStatus();

    void update();
    void init();
    void check_inputs();
    void draw();

private:
    bool tooling_on;

    bool mouseScaleReady = false;
    bool mouseScaleMode = false;
    bool mouseMoveRect = false;

    Rectangle current_rec;
    Vector2 mOffset;
    Entity current_en;

};