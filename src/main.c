#include "raylib.h"
#include "types.h"
#include "player.h"
#include "settings.h"
#include "render2d.h"
#include "raycast.h"

int main(void) {
    InitWindow(screenWidth, screenHeight, "Raylib Project");
    // InitWalls(10);

    Player player = {.position.x = mapWidth / 2, .position.y = mapHeight / 2, .angle = 0};
    Ray2D rays[raysCount];
    RaycastHit hits[raysCount];

    while (!WindowShouldClose()) {
        BeginDrawing();
        ClearBackground(BLACK);

        MovePlayer(&player);
        Raycast(player, rays, hits);
        Draw2D(player, rays, hits);
        
        DrawFPS(5, 5);
        EndDrawing();
    }

    // FreeWalls();
    CloseWindow();
    return 0;
}
