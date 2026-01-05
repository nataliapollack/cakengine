#pragma once
#include "raylib.h"
#include "Events.hpp"

#include "Game.h"

class Audio
{
public:
    void init();
    void load();
    void unload();

    void update();

    void PlayMusic(int theme);

    void TriggerAudioSFX(Event& event);
    void TriggerAudioStopSFX(Event& event);
    void TriggerMusicShift(Event& event);

private:
    bool end_game;
    int current_theme;
    Music themes[3];

    Sound sounds[AUDIO_COUNT];
};