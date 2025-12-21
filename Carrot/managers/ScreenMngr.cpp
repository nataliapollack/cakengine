#include "ScreenMngr.h"

#include "Coordinator.hpp"
#include "Game.h"

extern Coordinator gCoordinator;

void ScreenManager::init()
{
    screen_height = GetScreenHeight();
    screen_width = GetScreenWidth();
    current_screen = MENU;

    gCoordinator.AddEventListener(METHOD_LISTENER(Events::Transition::DOOROPEN, ScreenManager::RemoveStatic));

    menu = LoadTexture("art/ui/MENU.png");
    button1 = LoadTexture("art/ui/BUTTON 1.png");
    button2 = LoadTexture("art/ui/BUTTON 2.png");

    whiteNoise = GenImageWhiteNoise(screen_width, screen_height, 0.5f);
    static_img = LoadTextureFromImage(whiteNoise);

    flip_ver = 1.0f;
    flip_hor = 1.0f;
    cycle = true;
    frames_counter = 0;

    remove_static = false;
    should_close = false;
}

void ScreenManager::ChangeScreen(int i)
{
    current_screen = i;
}

void ScreenManager::draw()
{
    if (current_screen == MENU)
    {
        Rectangle source = { 0, 0, menu.width, menu.height };
        Rectangle dest = { 0, 0, screen_width , screen_height };
        Vector2 origin = Vector2{ 0,0 };

        DrawTexturePro(menu, source, dest, origin, 0, WHITE);


        DrawTexturePro(button1,source, dest, origin, 0.0f, WHITE);
        DrawTexturePro(button2, source, dest, origin, 0.0f, WHITE);

        DrawText("Click to Play.", 300, 250, 40.0f, PINK);
        DrawText("ESC to Close.", 350, 500, 30.0f, RED);
    }
    if (current_screen == GAME && !remove_static)
    {

        Rectangle source = { 0, 0, static_img.width * flip_hor, static_img.height * flip_ver };
        Rectangle dest = { 0, 0, static_img.width , static_img.height  };
        Vector2 origin = Vector2{ 0,0 };

        DrawTexturePro(static_img, source, dest, origin, 0, ColorAlpha(WHITE, 0.09));
    }
    if (current_screen == CREDITS)
    {
        ClearBackground(WHITE);
        DrawText("Made in C++ using Raylib!", 100, 50, 20.0f, BLACK);
        DrawText("All code was done by songbirdie", 100, 150, 30.0f, BLACK);
        DrawText("All art was done by Alex Soverign", 100, 250, 30.0f, BLACK);
        DrawText("All audio was done by sno", 100, 350, 30.0f, BLACK);
        DrawText("Thank you for playing!", 100, 450, 30.0f, BLACK);
        DrawText("Hit Enter to close the game.", 100, 500, 20.0f, BLACK);
    }
}

void ScreenManager::update()
{
    if (current_screen == GAME && !remove_static)
    {
        if (frames_counter >= (60 / 5.0f))
        {
            frames_counter = 0;

            if (cycle)
            {
                for (int i = 0; i < 3; i++)
                {
                    flip_hor *= -1;
                    //decision_bubbles[i].flip_ver *= -1;
                }
                cycle = !cycle;
            }
            else
            {
                for (int i = 0; i < 3; i++)
                {
                    //decision_bubbles[i].flip_hor *= -1;
                    flip_ver *= -1;
                }
                cycle = !cycle;
            }
        }
        frames_counter++;
    }

    if (current_screen == CREDITS)
    {
        if (IsKeyPressed(KEY_ENTER))
            should_close = true;
    }

    if (current_screen == MENU)
    {
        if (IsMouseButtonDown(MOUSE_BUTTON_LEFT))
        {
            current_screen = GAME;
        }
    }
}

void ScreenManager::unload()
{
    UnloadTexture(menu);
    UnloadTexture(button1);
    UnloadTexture(button2);
    UnloadImage(whiteNoise);
}

bool ScreenManager::GetClose()
{
    if (should_close)
    {
        should_close = false;
        current_screen = MENU;
        return true;
    }

    return should_close;
}

int ScreenManager::GetScreen()
{
    return current_screen;
}

void ScreenManager::RemoveStatic(Event& event)
{
    remove_static = true;
}

