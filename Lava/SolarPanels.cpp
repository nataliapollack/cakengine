#include "SolarPanels.h"
#include "Coordinator.hpp"
#include "raylib.h"
#include "rlgl.h"
#include "Lava.h"
#include "ScreenManager.h"

extern Coordinator gCoordinator;

void SolarPanel::init()
{
    toggle = false;
    completed = false;
    placed_panel_count = -1;

    for (int i = 0; i < 5; i++)
    {
        std::vector<bool> temp;
        for (int j = 0; j < 5; j++)
        {
            temp.push_back(false);
        }

        solar_grid.push_back(temp);

        current_tiles[i] = tile{ 0, 0 };
    }
}



void SolarPanel::update()
{
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
                    solar_grid[i][j] = !solar_grid[i][j];
                    placed_panel_count++;

                    // means we replace first placed one
                    if (placed_panel_count >= 5)
                    {
                        placed_panel_count = -1;
                        current_tiles[placed_panel_count + 1];

                    }
                    current_tiles[placed_panel_count + 1].col = i;
                    current_tiles[placed_panel_count + 1].row = j;
                }
            }

            current_box.y += current_box.height;
        }

        current_box.x += current_box.width;
        current_box.y = 0;
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
            if (solar_grid[i][j])
            {
                col = GREEN;
                DrawRectangleRec(current_box, col);
            }
            else
            {
                col = DARKGREEN;
                DrawRectangleLinesEx(current_box, 10, col);
            }

            current_box.y += current_box.height;
        }

        current_box.x += current_box.width;
        current_box.y = 0;
    }
}

bool SolarPanel::active()
{
    return toggle;
}

void SolarPanel::StartMinigame(Event& event)
{
    if (!completed)
    {
        toggle = true;

        Event screen(Events::Game::SCREEN_CHANGE);
        screen.SetParam(Events::Game::SCREEN_ID, SOLAR_SCREEN);

        gCoordinator.SendEvent(screen);
    }
}

void SolarPanel::StartNewDay(Event& event)
{
    toggle = false;
    completed = false;
}