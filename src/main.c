#include "raylib.h"
#include "player.h"
// #include "render.h"
#include "settings.h"
#include "render2d.h"

int main(void) {
    InitWindow(screenWidth, screenHeight, "Raylib Project");
    // InitWalls(10);

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);
        
        MovePlayer();
        Draw2D();
        
        DrawFPS(5, 5);
        EndDrawing();
    }

    // FreeWalls();
    CloseWindow();
    return 0;
}
