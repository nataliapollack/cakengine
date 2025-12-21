#pragma once
#include "raylib.h"
#include "Events.hpp"

class ObjPlacer
{
public:
    void init();
    void place_ski_objs();
    void place_player();
    void place_finish_line(Event &event);

    void unload();
private:
    Texture2D dog;
    Texture2D tree;
    Texture2D flags;
};