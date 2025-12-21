#pragma once
#include "raylib.h"

struct RenderBox
{
    float width;
    float height;
    Color color;
};

struct RenderText
{
    const char* string;
};

struct RenderBox2D
{
    Rectangle box;
    Color color;
};