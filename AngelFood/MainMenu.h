#pragma once

#include "raylib.h"

#include "Timer.hpp"
#include "Events.hpp"

enum Button : int
{
    START,
    CREDITS
};

enum Scene : int
{
    MAINMENU,
    GAMEPLAY,
    OUTRO
};

class MainMenu
{
public:
    void init();
    void updateIntro(float dt);
    void drawIntro();
    void updateOutro(float dt);
    void drawOutro();

    Scene GetScene() const;
    void SetScene(Scene newScene);

    bool GetOutro() const;
private:
    void SetOutro(Event& event);

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
    bool outro_running;
};
