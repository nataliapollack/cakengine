#pragma once

#include "Types.hpp"
#include "raylib.h"

static float FLOOR_Y = 350.0f;

enum TEXTURE_LIST
{
    player_idle,
    girl1_chibi,
    girl2_chibi,
    girl3_chibi,
    fake_chibi,
    hallway_bg,
    bedroom_bg,
    big_cam,
    lil_cam,
    light_1,
    light_2,
    top_light1,
    top_light2,
    top_light3,
    chair,
    red_exit,
    count
};

class LevelManager
{
public:

    void init();
    void load();
    void register_components();

    void set_signatures();

    void PlaceObjectsRoom();
    void PlaceObjectsHallway();
    void PlaceFinalDay();
    void PlaceAlmostLastDay();
    void PlaceObjects();
    void ClearEntities();

    void TriggerRoomChange(Event& event);
    void TriggerNewDay(Event& event);

    void TriggerDoorUnlocked(Event& event);

    void unload();

private:
    int day;
    int active_entities;
    int current_room;
    Entity entities_list[20];

    Texture entity_textures[TEXTURE_LIST::count];
};