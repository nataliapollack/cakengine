#pragma once
#include "raylib.h"

#include "Events.hpp"

enum BUTTON_TYPES
{
    PLAY,
    QUIT,
};

struct button
{
    bool state;
    const char* text;
    BUTTON_TYPES type;
    Rectangle box;
};

class Menus
{
public:
    void init();
    void update();
    void draw();

    bool get_status();
    bool get_reset();
    bool get_menu();

    void reset();

    void set_scores(float score);

    void triggerMenu(Event& event);

private:
    bool quit_game;
    bool reset_game;
    bool in_menu;

    float high_score;
    float current_score;

    button buttons[2];
};