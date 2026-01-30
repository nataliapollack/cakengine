#pragma once
#include "Events.hpp"

enum SCREENS
{
    MENU,
    GAME,
    AUDIO_FREQ,
    MAZE,
    SOLAR_TILE,
    CAMERAS,
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