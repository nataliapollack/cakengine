#pragma once
#include "System.hpp"
#include "raylib.h"

enum COMPONENTS_LIST
{
    PLAYER,
    WALL,
    GRAVE
};

class Tooling
{
public:
    void init();

    void serialize();
    void deserialize();

    bool GetToolStatus();

    void update();
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