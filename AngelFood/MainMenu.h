#pragma once

#include "raylib.h"

#include "Timer.hpp"
#include "Events.hpp"
#include "TextManager.h"

enum Button : int
{
    START,
    CREDITS
};

enum Scene : int
{
    MAINMENU,
    GAMEPLAY,
    OUTRO,
    CREDITS_SCENE
};

class MainMenu
{
public:
    void init();
    void updateIntro(float dt);
    void drawIntro();
    void updateOutro(float dt);
    void drawOutro(TextManager& txtManager);

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

    Texture2D cut1;
    Texture2D cut2;
    Texture2D cut3;

    Timer outro_time;
    Timer final_time;

    Timer fade_time;
    Timer gap_time;
    Timer illus_time;
    Timer start_time;

    Button curr_button;
    Scene curr_scene;

    bool start_game;
    bool change_illus;
    bool outro_running;
    bool final;
};
