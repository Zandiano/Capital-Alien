#include "raylib.h"

#include "game_logic.hpp"
#include "constants.hpp"


int main()
{
    InitWindow(ScreenW, ScreenH, "Capital Alien");

    SetTargetFPS(60);

    Camera3D camera = {
        {0,1,0}, 
        {0,0,2}, 
        {0,1,0}, 
        60, 
        CAMERA_PERSPECTIVE
    };
    
    Init();
    while (!WindowShouldClose()){
        UpdatePre();
        Update();
        UpdatePost();

        ClearBackground(RAYWHITE);
        
        BeginMode3D(camera);
        Render3D();
        EndMode3D();

        BeginDrawing();
        Render2D();
        EndDrawing();
        
        Debug();
        _sleep(10);
        std::system("cls");
    }
    CloseWindow();

    return 0;
}