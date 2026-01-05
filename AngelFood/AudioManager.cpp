#include "AudioManager.h"

#include "Coordinator.hpp"

#include "Game.h"

extern Coordinator gCoordinator;

void Audio::load()
{
    current_theme = 0;
    themes[0] = LoadMusicStream("audio/Ica Piano 2.mp3");
    themes[1] = LoadMusicStream("audio/Ica Credits.mp3");
    themes[2] = LoadMusicStream("audio/Ica Menu Edited.wav");

    sounds[SNAKE_SONG] = LoadSound("audio/Snekkoo.wav");

    sounds[SPARK_COLLECT] = LoadSound("audio/Snekkoo.wav");
    sounds[SPARK_SPAWN] = LoadSound("audio/Snekkoo.wav");

    sounds[STEP1] = LoadSound("audio/Footstep 1.wav");
    sounds[STEP2] = LoadSound("audio/Footstep 2.wav");
    sounds[JUMP_SFX] = LoadSound("audio/jump.wav");
    sounds[GLIDE_SFX] = LoadSound("audio/Float.wav");
    sounds[HURT_SFX] = LoadSound("audio/yeowch.wav");

    sounds[ITEM_COLLECT] = LoadSound("audio/Item collect.wav");
}

void Audio::update()
{
    UpdateMusicStream(themes[current_theme]);
}

void Audio::unload()
{
    for (int i = 0; i < AUDIO_COUNT; i++)
    {
        UnloadSound(sounds[i]);
    }

    UnloadMusicStream(themes[0]);
    UnloadMusicStream(themes[1]);
    UnloadMusicStream(themes[2]);
}

void Audio::TriggerAudioSFX(Event& event)
{
    AUDIO ev = event.GetParam<AUDIO>(Events::Audio::ASSET);

    PlaySound(sounds[ev]);
}

void Audio::TriggerAudioStopSFX(Event& event)
{
    AUDIO ev = event.GetParam<AUDIO>(Events::Audio::ASSET);

    StopSound(sounds[ev]);
}

void Audio::PlayMusic(int theme)
{
    if (current_theme != theme)
    {
        if (IsMusicStreamPlaying(themes[current_theme]))
        {
            StopMusicStream(themes[current_theme]);
        }
        current_theme = theme;

        PlayMusicStream(themes[current_theme]);
    }
}

void Audio::TriggerMusicShift(Event& event)
{
    int theme = event.GetParam<int>(Events::Audio::ASSET);

    if (current_theme != theme)
    {
        if (IsMusicStreamPlaying(themes[current_theme]))
        {
            StopMusicStream(themes[current_theme]);
        }

        current_theme = theme;
        PlayMusicStream(themes[current_theme]);
    }
}

void Audio::init()
{
    end_game = false;

    gCoordinator.AddEventListener(
        METHOD_LISTENER(Events::Audio::TRIGGEREDSFX, Audio::TriggerAudioSFX));

    gCoordinator.AddEventListener(
        METHOD_LISTENER(Events::Audio::STOPSFX, Audio::TriggerAudioStopSFX));

    gCoordinator.AddEventListener(
        METHOD_LISTENER(Events::Audio::STARTMUSIC, Audio::TriggerMusicShift));
}