#include "raylib.h"

#include "../headers/frameHandle.hpp"
#include "../headers/constants.hpp"
#include "../headers/constants.hpp"


int main()
{
    InitWindow(CONSTANT::ScreenW, CONSTANT::ScreenH, "Capital Alien");

    SetTargetFPS(60);               

    Camera3D camera = {
        position: {0,1,0}, 
        target: {0,0,2}, 
        up: {0,1,0}, 
        fovy: 60, 
        projection: CAMERA_PERSPECTIVE
    };

    while (!WindowShouldClose()){   
        UpdatePre();
        Update();
        UpdatePost();

        BeginDrawing();
        BeginMode3D(camera);
            ClearBackground(RAYWHITE);
            Render();
        EndMode3D();
        EndDrawing();
        std::system("cls");
    }
    CloseWindow();        

    return 0;
}