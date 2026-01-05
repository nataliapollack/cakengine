#include "AudioManager.h"

#include "Coordinator.hpp"

#include "Game.h"
#include <iostream>

extern Coordinator gCoordinator;

void Audio::load()
{
    current_theme = 0;
    themes[0] = LoadMusicStream("audio/Ica Piano 2.mp3");
    themes[1] = LoadMusicStream("audio/Ica Credits.mp3");
    themes[2] = LoadMusicStream("audio/Ica Menu Edited.wav");

    for (size_t i = 0; i < 3; ++i)
    {
        SetMusicVolume(themes[i], 0.75f);
    }

    sounds[SNAKE_SONG] = LoadSound("audio/Snekkoo.wav");

    sounds[SPARK_COLLECT] = LoadSound("audio/Spark Collect.wav");
    sounds[SPARK_SPAWN] = LoadSound("audio/Spark Spawn.wav");

    sounds[STEP1] = LoadSound("audio/Footstep 1.wav");
    sounds[STEP2] = LoadSound("audio/Footstep 2.wav");
    sounds[JUMP_SFX] = LoadSound("audio/jump.wav");
    sounds[GLIDE_SFX] = LoadSound("audio/Float.wav");
    sounds[HURT_SFX] = LoadSound("audio/yeowch.wav");

    sounds[ITEM_COLLECT] = LoadSound("audio/Item collect.wav");
}

void Audio::update(float dt)
{
    UpdateMusicStream(themes[current_theme]);

    if (transition_music)
    {
        float percent = (transition_time.count() / transition_time.time());
        if (transition_time.update(dt))
        {
            transition_music = false;
        }

        if (percent < 0.5f)
        {
            float new_volume = (1.0f - (percent / 0.5f)) * 0.75f;
            SetMusicVolume(themes[current_theme], new_volume);

            //std::cout << "last: " << new_volume << "\n";
        }
        else if (percent > 0.5f)
        {
            if (first_track == true)
            {
                first_track = false;
                // switch
                if (IsMusicStreamPlaying(themes[current_theme]))
                {
                    StopMusicStream(themes[current_theme]);
                    SetMusicVolume(themes[current_theme], 0.75f);
                }

                current_theme = new_theme;
                PlayMusicStream(themes[current_theme]);
            }
            float new_volume = ((percent - 0.5f) / 0.5f) * 0.75f;
            SetMusicVolume(themes[current_theme], new_volume);
            //std::cout << "new: " << new_volume << "\n";
        }


        //std::cout << new_volume << "\n";
    }

    if (IsSoundPlaying(sounds[SNAKE_SONG]))
    {
        SetMusicVolume(themes[current_theme], 0.25f);
    }
    else
    {
        SetMusicVolume(themes[current_theme], 0.75f);
    }
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
        if (transition_time.is_running()) return;
        transition_time.start();
        new_theme = theme;
        transition_music = true;
        first_track = true;
    }
}

void Audio::init()
{
    end_game = false;
    transition_music = false;
    transition_time = Timer(6.0f);

    gCoordinator.AddEventListener(
        METHOD_LISTENER(Events::Audio::TRIGGEREDSFX, Audio::TriggerAudioSFX));

    gCoordinator.AddEventListener(
        METHOD_LISTENER(Events::Audio::STOPSFX, Audio::TriggerAudioStopSFX));

    gCoordinator.AddEventListener(
        METHOD_LISTENER(Events::Audio::STARTMUSIC, Audio::TriggerMusicShift));
}