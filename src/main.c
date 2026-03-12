#include "raylib.h"
#include "player.h"
#include "render.h"
#include "settings.h"

int main(void) {
    InitWindow(screenWidth, screenHeight, "Raylib Project");
    InitRandomWalls(10);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        
        MovePlayer();
        DrawWallsByRays(player);
        
        DrawFPS(5, 5);
        EndDrawing();
    }

    FreeWalls();
    CloseWindow();
    return 0;
}
