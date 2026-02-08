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
    WIN,
    LOSE
};

class ScreenManager
{
public:
    void init();

    void ChangeScreen(Event& event);

    SCREENS GetScreen();

    void SetScreen(SCREENS screen);

    void UpdateFadeTransition(float dt); 
    void FadeIn(float dt);
    void FadeOut(float dt);
    bool GetIsFading();
    void StartFade(); 



    void UpdateWinLoseScreen(float dt);
    bool GetWindowClosed(); 

private:
    SCREENS current_screen;
    float fadeTimerDefault = 1.0f;
    float fadeTimerRemaining = fadeTimerDefault;
    bool IsFading = true;


    float timeToClose = 3.0f; 
    bool EndScreenComplete = false; 

    bool WinScreen = false;
  

    };