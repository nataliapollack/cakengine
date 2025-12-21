#include "AudioMngr.h"

#include "Coordinator.hpp"
#include "Events.hpp"

#include "Game.h"

extern Coordinator gCoordinator;

void AudioManager::init()
{
    gCoordinator.AddEventListener(METHOD_LISTENER(Events::Transition::DOOROPEN, AudioManager::TriggerAudioStop));
    gCoordinator.AddEventListener(METHOD_LISTENER(Events::Audio::SOUND, AudioManager::PlaySoundFx));
    play_jangle = false;
    in_hall = false;
}

void AudioManager::load()
{
    for (int i = 0; i < AUDIO::count_audio; i++)
    {
        all_audio[i] = LoadSound(TextFormat("audio/%i.mp3", i));
    }

    music_one = LoadMusicStream("audio/jingle.mp3");
    music_two = LoadMusicStream("audio/jangle.mp3");
}

void AudioManager::unload()
{
    for (int i = 0; i < AUDIO::count_audio; i++)
    {
        UnloadSound(all_audio[i]);
    }

    UnloadMusicStream(music_one);
    UnloadMusicStream(music_two);
}

void AudioManager::update()
{
    if (play_jangle)
    {
        UpdateMusicStream(music_two);
    }
    else
    {
        if (!IsSoundPlaying(all_audio[transition2]))
            UpdateMusicStream(music_one);
    }
}

void AudioManager::PlaySoundFx(Event& event)
{
    int sound = event.GetParam<int>(Events::Audio::SOUND);

    if (sound == transition1)
    {
        PauseMusicStream(music_one);
    }
    if (sound == transition2)
    {
        PlayMusicStream(music_one);
    }

    if (sound == audience_app)
    {
        PauseMusicStream(music_two);
    }

    PlaySound(all_audio[sound]);
}

void AudioManager::TriggerAudioStop(Event& event)
{
    play_jangle = true;
    PauseMusicStream(music_one);
    PlayMusicStream(music_two);
    SetMusicVolume(music_two, 0.25);
}
