#pragma once
#include "raylib.h"
#include "Events.hpp"

#include "Game.h"
#include "Timer.hpp"

class Audio
{
public:
    void init();
    void load();
    void unload();

    void update(float dt);

    void PlayMusic(int theme);

    void TriggerAudioSFX(Event& event);
    void TriggerAudioStopSFX(Event& event);
    void TriggerMusicShift(Event& event);

private:
    bool end_game;
    int current_theme;
    Music themes[3];

    Timer transition_time;
    int new_theme;
    bool transition_music;
    bool first_track;

    Sound sounds[AUDIO_COUNT];
};