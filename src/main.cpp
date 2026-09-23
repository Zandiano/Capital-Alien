#include "raylib.h"

#include "../headers/game_logic.hpp"
#include "../headers/constants.hpp"


int main()
{
    InitWindow(ScreenW, ScreenH, "Capital Alien");

    SetTargetFPS(60);

    Camera3D camera = {
        position: {0,1,0}, 
        target: {0,0,2}, 
        up: {0,1,0}, 
        fovy: 60, 
        projection: CAMERA_PERSPECTIVE
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