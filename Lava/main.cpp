#include "Coordinator.hpp"
#include "raylib.h"
#include "rlgl.h"
#include "raymath.h"
#include <algorithm> 

int main()
{
    /*** Window Initialization *************************************************************/
    InitWindow(900, 600, "WASTELAND");

    while (!WindowShouldClose())
    {
        // update
        float deltaTime = 1.0f / 60.0f;

        //draw

        BeginDrawing();

        ClearBackground(RED);

        EndDrawing();
    }

    CloseWindow();
}