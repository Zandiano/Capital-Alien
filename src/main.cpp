#include "raylib.h"

#include "../headers/frameHandle.hpp"

#define ScreenW 900
#define ScreenH 900



int main()
{
    InitWindow(ScreenW, ScreenH, "Capital Alien");

    SetTargetFPS(60);               

    while (!WindowShouldClose()){   
        UpdatePre();
        Update();
        UpdatePost();

        BeginDrawing();
            ClearBackground(RAYWHITE);
            Render();
        EndDrawing();
    }
    CloseWindow();        

    return 0;
}