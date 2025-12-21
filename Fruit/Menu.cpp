#include "Menu.h"

#include "Game.h"
#include "Coordinator.hpp"

extern Coordinator gCoordinator;

void Menus::init()
{
    gCoordinator.AddEventListener(METHOD_LISTENER(Events::Obstacles::FINISH, Menus::triggerMenu));

    float w = GetScreenWidth();
    float h = GetScreenHeight();

    high_score = 0;
    current_score = 0;

    quit_game = false;
    reset_game = false;
    in_menu = true;

    buttons[0].state = false;
    buttons[0].text = "PLAY";
    buttons[0].type = PLAY;
    buttons[0].box = { w / 4, 50, 400, 100 };

    buttons[0].state = false;
    buttons[1].text = "QUIT";
    buttons[1].type = QUIT;
    buttons[1].box = { w / 4, 200, 400, 100};
}

void Menus::update()
{
    Vector2 mousepos = GetMousePosition();

    if (CheckCollisionPointRec(mousepos, buttons[0].box))
    {
        buttons[0].state = true;
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            reset_game = true;
            in_menu = false;
        }
    }
    else
    {
        buttons[0].state = false;
    }

    if (CheckCollisionPointRec(mousepos, buttons[1].box))
    {
        buttons[1].state = true;
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            quit_game = true;
        }
        
    }
    else
    {
        buttons[1].state = false;
    }
}

void Menus::draw()
{
    DrawRectangle(0, 0, GetScreenWidth(), GetScreenHeight(), ColorAlpha(GRAY, 0.6));

    if (buttons[0].state)
    {
        DrawRectangleRec(buttons[0].box, ColorAlpha(DARKGREEN, 0.5));
    }
    else
    {
        DrawRectangleRec(buttons[0].box, ColorAlpha(GREEN, 0.5));
    }
    DrawText(buttons[0].text, buttons[0].box.x + 50, buttons[0].box.y + 20, 60, BLACK);

    if (buttons[1].state)
    {
        DrawRectangleRec(buttons[1].box, ColorAlpha(DARKGREEN, 0.5));
    }
    else
    {
        DrawRectangleRec(buttons[1].box, ColorAlpha(GREEN, 0.5));
    }
    DrawText(buttons[1].text, buttons[1].box.x + 50, buttons[1].box.y + 20, 60, BLACK);

    DrawText(TextFormat("HIGH SCORE: %04.f", high_score), buttons[0].box.x, 300, 30, BLACK);
    DrawText(TextFormat("SCORE: %04.f", current_score), buttons[0].box.x, 350, 30, BLACK);
}

bool Menus::get_status()
{
    return quit_game;
}

bool Menus::get_reset()
{
    return reset_game;
}

bool Menus::get_menu()
{
    return in_menu;
}

void Menus::reset()
{
    in_menu = false;
    quit_game = false;
    reset_game = false;
}

void Menus::set_scores(float score)
{
    current_score = score;

    if (current_score > high_score)
    {
        high_score = current_score;
    }
}

void Menus::triggerMenu(Event& event)
{
    in_menu = true;
}