#pragma once 
#include "raylib.h"

#include "Coordinator.hpp"
#include "systems/RenderSystem.h"
#include "systems/PlayerMovementSystem.h"
#include "systems/CollisionSystem.h"
#include "systems/DialogueSystem.h"

#include "managers/LevelMngr.h"
#include "managers/DecisionMngr.h"
#include "managers/TransitionMngr.h"
#include "managers/MinigameMngr.h"
#include "managers/InnerDialogueMngr.h"
#include "managers/ScreenMngr.h"
#include "managers/AudioMngr.h"

#include "Game.h"

Coordinator gCoordinator;

void temp_main()
{
    LevelManager mgnr;
    InitWindow(900, 600, "YOUR FAVORITE TUTOR");
    SetTargetFPS(60);

    // wow this is bad, make a serializer / deserializer to load this shit in the init or something nat
    gCoordinator.init();

    mgnr.register_components();

    auto movement_system = gCoordinator.RegisterSystem<PlayerMovementSystem>();
    auto render_sys = gCoordinator.RegisterSystem<RenderSystem>();
    auto collision_sys = gCoordinator.RegisterSystem<CollisionSystem>();


    DialogueSystem dialog_sys;
    dialog_sys.init();

    DecisionMngr decisions;
    decisions.init();

    MinigameManager minigame;
    minigame.init();
    minigame.PlaceNotes();

    mgnr.set_signatures();

    render_sys->init();

    ////////////////////////////////////////////// while in game
    while (!WindowShouldClose())
    {
        float deltaTime = GetFrameTime();

        // UPDATE CALLS
        minigame.update();

        //if (!dialog_sys.GetDialogue() && !decisions.GetDecisionStatus())
        //{
        //    render_sys->update();
        //    movement_system->update(deltaTime);
        //    collision_sys->CheckCollisions();
        //}

        //dialog_sys.update();
        //decisions.update();


     // DRAW CALLS /////////////////////////////////////////////////////

        ClearBackground(BLUE);
        // temp tb  h 
        BeginDrawing();

       // else
        //{
        //    render_sys->draw();
        //}


        //if (dialog_sys.GetDialogue())
        //{
        //    dialog_sys.draw();
        //}
        //if (decisions.GetDecisionStatus())
        //{
        //    decisions.draw();
        //}


        minigame.draw();

        EndDrawing();
    }
}

int main()
{

/*** Window Initialization *************************************************************/
    InitWindow(900, 600, "Your Favorite Tutor");
    SetTargetFPS(60);

    InitAudioDevice();

    gCoordinator.init();

/*** Managers / Systems Creation *************************************************************/
    LevelManager level_mngr;
    DecisionMngr dec_mngr;
    TransitionManager transition_mngr;
    MinigameManager minigame_mngr;
    InnerDialogue thoughts_mngr;
    DialogueSystem dialog_mngr;
    ScreenManager screen_mngr;
    AudioManager audio_mngr;

    auto movement_sys = gCoordinator.RegisterSystem<PlayerMovementSystem>();
    auto render_sys = gCoordinator.RegisterSystem<RenderSystem>();
    auto collision_sys = gCoordinator.RegisterSystem<CollisionSystem>();

/*** Managers loads  *************************************************************/
    level_mngr.load();
    dialog_mngr.load();
    minigame_mngr.load();
    audio_mngr.load();

/*** Managers / Systems Initialization *************************************************************/
    level_mngr.init();
    audio_mngr.init();
    screen_mngr.init();
    dec_mngr.init();
    transition_mngr.init();
    minigame_mngr.init();
    thoughts_mngr.init();
    dialog_mngr.init();

    movement_sys->init();
    render_sys->init();

/*** Pre-Game Placements *************************************************************/
   level_mngr.PlaceObjectsRoom();

   Event dec_happen(Events::Thoughts::THINK);
   gCoordinator.SendEvent(dec_happen);
   // level_mngr.PlaceFinalDay();
  //  level_mngr.PlaceObjects();

/*** While in Game Loop *************************************************************/
    while (!WindowShouldClose() && !screen_mngr.GetClose())
    {
        float deltaTime = GetFrameTime();

/*** Update Calls *************************************************************/

        if (transition_mngr.GetGanmeOver())
        {
            screen_mngr.ChangeScreen(CREDITS);
        }

        if (screen_mngr.GetScreen() == GAME)
        {
            if (!dec_mngr.GetDecisionStatus() && !thoughts_mngr.GetThoughtsStatus() && !dialog_mngr.GetDialogue() && !minigame_mngr.GetMinigameStatus())
            {
                render_sys->update();
                collision_sys->CheckCollisions();
            }

            movement_sys->update(deltaTime);

            dec_mngr.update();
            transition_mngr.update();
            minigame_mngr.update();
            thoughts_mngr.update();
            dialog_mngr.update();
        }

        screen_mngr.update();
        audio_mngr.update();

/*** Draw Calls ***************************************************************/
        BeginDrawing();
        ClearBackground(BLACK);

        if (screen_mngr.GetScreen() == GAME)
        {
            if (!minigame_mngr.GetMinigameStatus())
            {
                movement_sys->BeginDrawCam();
                render_sys->draw();
                movement_sys->EndDrawCam();
            }

            dec_mngr.draw();
            minigame_mngr.draw();
            thoughts_mngr.draw();
            transition_mngr.draw_fade();
        }

        screen_mngr.draw();
        dialog_mngr.draw();


        EndDrawing();

    }

/*** Unload Calls ***************************************************************/
    level_mngr.unload();
    dialog_mngr.unload();
    screen_mngr.unload();
    minigame_mngr.unload();
    audio_mngr.unload();

    CloseAudioDevice();
    CloseWindow();
}