#pragma once
#include "raylib.h"
#include "Events.hpp"

class Audio
{
public:
    void load();
    void unload();

    void TriggerAudioSFX(Event& event);
    void TriggerMusicShift(Event& event);

private:


};