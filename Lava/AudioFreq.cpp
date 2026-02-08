#include "AudioFreq.h"
#include "Coordinator.hpp"

#include "Lava.h"

#include "ScreenManager.h"
#include "AudioMAnager.h"

extern Coordinator gCoordinator;

extern int gCurrentDay;

void AudioFreq::init()
{
    Correct_angle = 1200;
    Current_angle = 1000;

    // Rectangle start = { 20.0f, 600 - 120.f , 200.0f, 100.0f };
     float screen_width = GetScreenWidth();
     float screen_height = GetScreenHeight();

     Rectangle start = { 0, 200.0f, 200.0f, 100.0f};
     float stride = GetScreenWidth() / WAVE_POINTS;
   //  Correct_sinePoints[WAVE_POINTS] = { 0 };

     for (int i = 0; i < WAVE_POINTS; i++)
     {
         float t = i / (float)(WAVE_POINTS - 1);
         float currentAngle = t * Correct_angle * DEG2RAD;
         Correct_sinePoints[i].x = (start.x + t * start.width) + (stride * i);
         Correct_sinePoints[i].y = start.y + start.height / 2.0f - sinf(currentAngle) * (start.height / 2.0f);

         float currentAngle2 = t * Current_angle * DEG2RAD;
         Current_sinePoints[i].x = (start.x + t * start.width) + (stride * i);
         Current_sinePoints[i].y = start.y + start.height / 2.0f - sinf(currentAngle2) * (start.height / 2.0f);
     }



     knobs_pos[0] = Vector2{ screen_width - 200, 500 };
     knobs_pos[1] = Vector2{ 200, 500 };
     completed = false;

     radius = 50.0f;

     begin_timer = false;
     timer = 3.0f;

     gCoordinator.AddEventListener(
         METHOD_LISTENER(Events::Collision::HIT_AUDIO, AudioFreq::StartMinigame));


     gCoordinator.AddEventListener(
         METHOD_LISTENER(Events::Time::DAY_BEGIN, AudioFreq::StartNewDay));
}

void AudioFreq::update()
{
    Vector2 mouse_pos = GetMousePosition();
    begin_timer = false;

    if (!completed)
    {
        Event audio(Events::Audio::PLAY_SOUND);
        audio.SetParam(Events::Audio::ASSET, RADIO_SOUND);
        gCoordinator.SendEvent(audio);
    }

    if (CheckCollisionPointCircle(mouse_pos, knobs_pos[1], radius))
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            Current_angle += 10.0f;
            Rectangle start = { 0, 200.0f, 200.0f, 100.0f };
            float stride = GetScreenWidth() / WAVE_POINTS;
            for (int i = 0; i < WAVE_POINTS; i++)
            {
                float t = i / (float)(WAVE_POINTS - 1);
                float currentAngle2 = t * Current_angle * DEG2RAD;
                Current_sinePoints[i].x = (start.x + t * start.width) + (stride * i);
                Current_sinePoints[i].y = start.y + start.height / 2.0f - sinf(currentAngle2) * (start.height / 2.0f);
            }
        }

        Event audio(Events::Audio::PLAY_SOUND);
        audio.SetParam(Events::Audio::ASSET, GetRandomValue(7, 12));
        gCoordinator.SendEvent(audio);
    }

    if (CheckCollisionPointCircle(mouse_pos, knobs_pos[0], radius))
    {
        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
        {
            Current_angle -= 10.0f;
            Rectangle start = { 0, 200.0f, 200.0f, 100.0f };
            float stride = GetScreenWidth() / WAVE_POINTS;
            for (int i = 0; i < WAVE_POINTS; i++)
            {
                float t = i / (float)(WAVE_POINTS - 1);
                float currentAngle2 = t * Current_angle * DEG2RAD;
                Current_sinePoints[i].x = (start.x + t * start.width) + (stride * i);
                Current_sinePoints[i].y = start.y + start.height / 2.0f - sinf(currentAngle2) * (start.height / 2.0f);
            }

            Event audio(Events::Audio::PLAY_SOUND);
            audio.SetParam(Events::Audio::ASSET, GetRandomValue(7, 12));
            gCoordinator.SendEvent(audio);
        }
    }

    if (Current_angle == Correct_angle)
    {
        if (!begin_timer)
        {
            timer = 3.0f;
        }
        begin_timer = true;
    }

    if (begin_timer)
    {
        timer -= 1 / 60.0f;
        if (timer <= 0)
        {
            completed = true;

            Event hints(Events::Hints::HINT_RECEIVED);
            gCoordinator.SendEvent(hints);

            Event screen(Events::Dialogue::AUDIO_FIXED);
            screen.SetParam(Events::Dialogue::ID, gCurrentDay);
            gCoordinator.SendEvent(screen);

            Event audio(Events::Audio::PLAY_SOUND);
            audio.SetParam(Events::Audio::ASSET, FAX);
            gCoordinator.SendEvent(audio);
        }
    }

    if (IsKeyPressed(KEY_BACKSPACE))
    {
        Event screen(Events::Game::SCREEN_CHANGE);
        screen.SetParam(Events::Game::SCREEN_ID, INSIDE);

        gCoordinator.SendEvent(screen);

        Event audio(Events::Audio::PLAY_SOUND);
        audio.SetParam(Events::Audio::ASSET, RADUIO_STOP);
        gCoordinator.SendEvent(audio);
    }

}

void AudioFreq::draw()
{

    DrawSplineLinear(Correct_sinePoints, WAVE_POINTS, 2.0f, GREEN);
    DrawSplineLinear(Current_sinePoints, WAVE_POINTS, 2.0f, ColorAlpha(RED, 0.75));

    DrawCircle(knobs_pos[0].x, knobs_pos[0].y, radius, LIGHTGRAY);
    DrawCircle(knobs_pos[1].x, knobs_pos[1].y, radius, LIGHTGRAY);

}

void AudioFreq::StartMinigame(Event& event)
{
    if (completed)
    {
        Event screen1(Events::Dialogue::TUTORIAL);
        screen1.SetParam(Events::Dialogue::ID, -1);
        gCoordinator.SendEvent(screen1);
        return;
    }

    toggle = true;

    Event screen(Events::Game::SCREEN_CHANGE);
    screen.SetParam(Events::Game::SCREEN_ID, AUDIO_FREQ);

    gCoordinator.SendEvent(screen);

    Event screen1(Events::Dialogue::TUTORIAL);
    screen1.SetParam(Events::Dialogue::ID, 2);
    gCoordinator.SendEvent(screen1);


    Event audio(Events::Audio::PLAY_SOUND);
    audio.SetParam(Events::Audio::ASSET, MINI_SFX);
    gCoordinator.SendEvent(audio);
}

void AudioFreq::StartNewDay(Event& event)
{
    Correct_angle = GetRandomValue(100, 500) * 10;
    Current_angle = 1000;

    // Rectangle start = { 20.0f, 600 - 120.f , 200.0f, 100.0f };
    float screen_width = GetScreenWidth();
    float screen_height = GetScreenHeight();

    Rectangle start = { 0, 200.0f, 200.0f, 100.0f };
    float stride = GetScreenWidth() / WAVE_POINTS;
    //  Correct_sinePoints[WAVE_POINTS] = { 0 };

    for (int i = 0; i < WAVE_POINTS; i++)
    {
        float t = i / (float)(WAVE_POINTS - 1);
        float currentAngle = t * Correct_angle * DEG2RAD;
        Correct_sinePoints[i].x = (start.x + t * start.width) + (stride * i);
        Correct_sinePoints[i].y = start.y + start.height / 2.0f - sinf(currentAngle) * (start.height / 2.0f);

        float currentAngle2 = t * Current_angle * DEG2RAD;
        Current_sinePoints[i].x = (start.x + t * start.width) + (stride * i);
        Current_sinePoints[i].y = start.y + start.height / 2.0f - sinf(currentAngle2) * (start.height / 2.0f);
    }

}