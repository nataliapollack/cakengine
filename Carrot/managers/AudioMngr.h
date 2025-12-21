#pragma once
#include "raylib.h"

#include "Events.hpp"

#include "Game.h"

class AudioManager
{
public:
    void init();
    void load();
    void unload();

    void update();

    void PlaySoundFx(Event& event);

    void TriggerAudioStop(Event& event);


private:
    bool play_jangle;
    bool in_hall;

    Music music_one;
    Music music_two;
    Sound all_audio[AUDIO::count_audio];
};