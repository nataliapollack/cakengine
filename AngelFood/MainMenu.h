#pragma once

#include "raylib.h"

#include "Timer.hpp"

enum Button : int
{
    START,
    CREDITS
};

enum Scene : int
{
    MAINMENU,
    GAMEPLAY
};

class MainMenu
{
public:
    void init();
    void update(float dt);
    void draw();

    Scene GetScene() const;
private:
    Texture2D title;
    Texture2D illus1;
    Texture2D illus2;
    Texture2D start;
    Texture2D credits;

    Timer fade_time;
    Timer gap_time;
    Timer illus_time;
    Timer start_time;

    Button curr_button;
    Scene curr_scene;

    bool start_game;
    bool change_illus;
};
