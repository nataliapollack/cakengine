#pragma once
#include "raylib.h"

struct RenderBox
{
    float width;
    float height;
    Color color;
    Texture image;
};

struct RenderBox2D
{
    Rectangle box;
    Color color;
};