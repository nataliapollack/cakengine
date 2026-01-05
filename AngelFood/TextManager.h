#pragma once
#include "Events.hpp"
#include "raylib.h"

class TextManager
{
public:
    void init();
    void update();
    void draw();

    void unload();

    void TriggerWalkingTutorial(Event& event);

    void TriggerDoubleJumpTutorial(Event& event);

    void TriggerCredits(Event& event);
    void EndCredits(Event& event);

private:
    Font text_font;
    float time_shown;
    bool credits;
    bool currently_showing;
    std::string current_string[4];
    std::string credit_str;
    int frame_counter;
    int string_counter;
};