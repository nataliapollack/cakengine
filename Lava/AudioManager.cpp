#include "AudioManager.h"

#include "raylib.h"
#include "Lava.h"

#include "Coordinator.hpp"

extern Coordinator gCoordinator;


void Audio::update()
{
    UpdateMusicStream(theme[current_theme]);
}

void Audio::load()
{
    for (int i = 0; i < AUDIO_COUNT; i++)
    {
        sfx[i] = LoadSound(TextFormat("audio/%i.wav", i);
    }

    theme[0] = LoadMusicStream("audio/mus/Inside music + ambience.wav");
    theme[1] = LoadMusicStream("audio/mus/Inside extra electrical ambience.wavv");
}

void Audio::unload()
{
    for (int i = 0; i < AUDIO_COUNT; i++)
    {
        UnloadSound(sfx[i]);
    }
}

void Audio::TriggerSFX(Event& event)
{
    AUDIO ev = event.GetParam<AUDIO>(Events::Audio::ASSET);

    if (!IsSoundPlaying(sfx[ev]))
        PlaySound(sfx[ev]);
}

void Audio::TriggerMusic(Event& event)
{
    int theme = event.GetParam<int>(Events::Audio::ASSET);

    if (current_theme != theme)
    {
        current_theme = theme;
    }
}


void Audio::init()
{
    current_theme = 0;

    gCoordinator.AddEventListener(
        METHOD_LISTENER(Events::Audio::PLAY_SOUND, Audio::TriggerSFX));

    gCoordinator.AddEventListener(
        METHOD_LISTENER(Events::Audio::PLAY_MUSIC, Audio::TriggerMusic));
}