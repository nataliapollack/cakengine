#include "AudioFreq.h"
#include "Coordinator.hpp"

#include "Lava.h"

#include "ScreenManager.h"

extern Coordinator gCoordinator;


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

     radius = 50.0f;

     begin_timer = false;
     timer = 3.0f;

     gCoordinator.AddEventListener(
         METHOD_LISTENER(Events::Collision::HIT_AUDIO, AudioFreq::StartMinigame));
}

void AudioFreq::update()
{
    if (completed)
        return;

    Vector2 mouse_pos = GetMousePosition();
    begin_timer = false;

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
        }
    }

    if (IsKeyPressed(KEY_BACKSPACE))
    {
        Event screen(Events::Game::SCREEN_CHANGE);
        screen.SetParam(Events::Game::SCREEN_ID, INSIDE);

        gCoordinator.SendEvent(screen);
    }

}

void AudioFreq::draw()
{

    DrawSplineLinear(Correct_sinePoints, WAVE_POINTS, 2.0f, GREEN);
    DrawSplineLinear(Current_sinePoints, WAVE_POINTS, 2.0f, ColorAlpha(RED, 0.75));

    DrawCircle(knobs_pos[0].x, knobs_pos[0].y, radius, BLACK);
    DrawCircle(knobs_pos[1].x, knobs_pos[1].y, radius, BLACK);

}

void AudioFreq::StartMinigame(Event& event)
{
    toggle = true;

    Event screen(Events::Game::SCREEN_CHANGE);
    screen.SetParam(Events::Game::SCREEN_ID, AUDIO_FREQ);

    gCoordinator.SendEvent(screen);
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