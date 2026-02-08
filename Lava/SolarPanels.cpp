#include "SolarPanels.h"
#include "Coordinator.hpp"
#include "raylib.h"
#include "rlgl.h"
#include "Lava.h"
#include "ScreenManager.h"

#include "raymath.h"

extern Coordinator gCoordinator;

void SolarPanel::init()
{
    toggle = false;
    placed_panel_count = 0;
    timer = 5;

    for (int i = 0; i < 5; i++)
    {
        std::vector<bool> temp;
        for (int j = 0; j < 5; j++)
        {
            temp.push_back(false);
        }

        solar_grid.push_back(temp);
    }

    for (int i = 0; i < 10; i++)
    {
        correct_panels[i].pos.x = GetRandomValue(0, 24);
        correct_panels[i].pos.y = GetRandomValue(0, 24);

        correct_panels[i].correct = false;
    }
}


void  SolarPanel::CheckCorrectness(float x, float y)
{
    for (int i = 0; i < 10; i++)
    {
        if (Vector2Equals(correct_panels[i].pos, Vector2(x, y)))
        {
            if (correct_panels[i].correct)
            {
                correct_panels[i].correct = false;

                Event energy(Events::Energy::ENERGY_DOWN);

                energy.SetParam(Events::Energy::ENERGY_TICK,
                    -20);
                gCoordinator.SendEvent(energy);
            }
            else
            {
                correct_panels[i].correct = true;

                Event energy(Events::Energy::ENERGY_UP);

                energy.SetParam(Events::Energy::ENERGY_TICK,
                    20);

                gCoordinator.SendEvent(energy);
            }
        }
    }
}

void SolarPanel::update()
{
    if (show_warning)
    {
        timer -= (1.f / 60);
        if (timer <= 0)
        {
            show_warning = false;
            timer = 5.0f;
        }
    }

    Rectangle current_box = { 0, 0, GetScreenWidth() / 5, GetScreenHeight() / 5 };

    Vector2 mouse_pos = GetMousePosition();

    for (int i = 0; i < solar_grid.size(); i++)
    {
        for (int j = 0; j < solar_grid[i].size(); j++)
        {
            if (CheckCollisionPointRec(mouse_pos, current_box))
            {
                if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                {
                    if (placed_panel_count >= 5)
                    {
                        show_warning = true;

                        if (solar_grid[i][j])
                        {
                            placed_panel_count--;
                            solar_grid[i][j] = false;
                            show_warning = false;
                            timer = 5.0f;
                        }
                    }
                    else
                    {
                        if (solar_grid[i][j])
                        {
                            placed_panel_count--;
                            solar_grid[i][j] = false;
                            show_warning = false;
                            timer = 5.0f;
                        }
                        else
                        {
                            placed_panel_count++;
                            solar_grid[i][j] = true;
                        }
                    }

                    CheckCorrectness(i, j);
                }
            }

            current_box.y += current_box.height;
        }

        current_box.x += current_box.width;
        current_box.y = 0;
    }

    if (IsKeyPressed(KEY_BACKSPACE))
    {
        Event screen(Events::Game::SCREEN_CHANGE);
        screen.SetParam(Events::Game::SCREEN_ID, OUTSIDE);

        gCoordinator.SendEvent(screen);
    }
}


void SolarPanel::draw()
{
    Rectangle current_box = { 0, 0, GetScreenWidth() / 5, GetScreenHeight() / 5 };
    Color col = DARKGREEN;

    for (int i = 0; i < solar_grid.size(); i++)
    {
        for (int j = 0; j < solar_grid[i].size(); j++)
        {
            if (show_hints)
            {
                if (Vector2Equals(correct_panels[i].pos, Vector2(i, j)))
                {
                    DrawRectangleRec(current_box, ColorAlpha(YELLOW, 0.5));
                }
            }

            if (solar_grid[i][j])
            {
                col = GREEN;
                DrawRectangleRec(current_box, ColorAlpha(col, 0.75));
            }
            //else
            {
                col = DARKGREEN;
                DrawRectangleLinesEx(current_box, 10, ColorAlpha(col, 0.75));
            }


            current_box.y += current_box.height;
        }

        current_box.x += current_box.width;
        current_box.y = 0;
    }

    if (show_warning)
    {
        DrawRectangle(275, 535, 350, 50, ColorAlpha(BLACK, 0.5));
        DrawText("Max Panels Placed.", 300, 550, 30.0f, RED);
    }
}

bool SolarPanel::active()
{
    return toggle;
}

void SolarPanel::StartMinigame(Event& event)
{
    toggle = true;

    Event screen(Events::Game::SCREEN_CHANGE);
    screen.SetParam(Events::Game::SCREEN_ID, SOLAR_TILE);

    gCoordinator.SendEvent(screen);

    Event screen1(Events::Dialogue::TUTORIAL);
    screen1.SetParam(Events::Dialogue::ID, 1);
    gCoordinator.SendEvent(screen1);
    
}

void SolarPanel::StartNewDay(Event& event)
{
    toggle = false;
    show_hints = false;

    // pick random correct panels

    for (int i = 0; i < 10; i++)
    {
        correct_panels[i].pos.x = GetRandomValue(0, 24);
        correct_panels[i].pos.y = GetRandomValue(0, 24);

        correct_panels[i].correct = false;
    }
}

void SolarPanel::ShowHint(Event& event)
{
    show_hints = true;
}