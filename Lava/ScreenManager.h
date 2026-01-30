#pragma once
#include "Events.hpp"

enum SCREENS
{
    MENU,
    INSIDE,
    OUTSIDE,
    AUDIO_FREQ,
    MAZE,
    SOLAR_TILE,
    CAMERAS,
    CREDITS,
};

class ScreenManager
{
public:
    void init();

    void ChangeScreen(Event& event);

    SCREENS GetScreen();

    void SetScreen(SCREENS screen);

private:
    SCREENS current_screen;
};