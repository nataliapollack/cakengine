#pragma once
#include "ScreenManager.h"
#include "Coordinator.hpp"

#include "raylib.h"
#include "Lava.h"


extern Coordinator gCoordinator;

void ScreenManager::init()
{
    gCoordinator.AddEventListener(
        METHOD_LISTENER(Events::Game::SCREEN_CHANGE, ScreenManager::ChangeScreen));
}

void ScreenManager::ChangeScreen(Event& event)
{
    SCREENS screen = event.GetParam<SCREENS>(Events::Game::SCREEN_ID);
    current_screen = screen;

    // Just for the sake of getting it working fast, i'm setting WinScreen here.
    // I know it should probably be done via event but I don't 
    // know enough about the event system to do it fast enough - Braedan
    if (screen == WIN)
    {
        WinScreen = true;
    }


    StartFade();
}

SCREENS ScreenManager::GetScreen()
{
    return current_screen;
}

void ScreenManager::SetScreen(SCREENS screen)
{
    current_screen = screen;
}



void ScreenManager::UpdateFadeTransition(float dt)
{
    if (!IsFading)
        return;

    // Fade timer counts up for fade in and down for fade out
 /*   if (fadeTimerRemaining < 0)
    {
        FadeIn(dt);
    }*/
    if (fadeTimerRemaining >= 0)
    {
        FadeOut(dt);
    }
}

//void ScreenManager::FadeIn(float dt)
//{
//    fadeTimerRemaining += dt;
//
//    if (fadeTimerRemaining >= 0)
//    {
//        fadeTimerRemaining = fadeTimerDefault;
//        return;
//    }
//    Color fadeColor = BLACK;
//    fadeColor.a = 255 * (fadeTimerRemaining / fadeTimerDefault);
//    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), fadeColor);
//}

void ScreenManager::FadeOut(float dt)
{
    fadeTimerRemaining -= dt;

    if (fadeTimerRemaining <= 0)
    {
        IsFading = false;
        return;
    }
    Color fadeColor = BLACK;
    fadeColor.a = 255 * (fadeTimerRemaining / fadeTimerDefault);
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), fadeColor);
}

bool ScreenManager::GetIsFading()
{
    return IsFading;
}

void ScreenManager::StartFade()
{
    fadeTimerRemaining = fadeTimerDefault;
    IsFading = true;
}

void ScreenManager::UpdateWinLoseScreen(float dt)
{
    timeToClose -= dt;
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), BLACK);
    WinScreen = true;
    if (WinScreen)
    {
        DrawText("YOU WIN!", GetScreenWidth() / 4, 150.0f, 100.0f, WHITE);
    }
    else
    {
        DrawText("YOU LOSE!", GetScreenWidth() / 4, 150.0f, 100.0f, WHITE);
    }

    if (timeToClose <= 0)
    {
        EndScreenComplete = true;
        CloseWindow();
    }
}

bool ScreenManager::GetWindowClosed()
{
    return EndScreenComplete;
}
