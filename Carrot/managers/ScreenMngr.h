#pragma once
#include "raylib.h"

#include "Events.hpp"

enum SCREENS
{
    MENU,
    GAME,
    CREDITS
};

class ScreenManager
{
public:
    void init();
    void draw();
    void update();

    void ChangeScreen(int i);
    void unload();

    int GetScreen();
    bool GetClose();

    void RemoveStatic(Event& event);

private:
    int selected_button;

    bool remove_static;
    bool should_close;
    bool cycle;
    float flip_ver;
    float flip_hor;
    int frames_counter;

    float screen_width;
    float screen_height;
    int current_screen;

    Image whiteNoise;
    Texture static_img;
    Texture menu;
    Texture button1;
    Texture button2;
};