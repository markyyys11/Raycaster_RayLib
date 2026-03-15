#include "raylib.h"
#include "types.h"
#include "player.h"
#include "settings.h"
#include "render2d.h"
#include "render3d.h"
#include "raycast.h"

int main(void) {
    InitWindow(screenWidth, screenHeight, "Raylib Project");

    Player player = {.position.x = mapWidth / 2, .position.y = mapHeight / 2, .angle = 0};
    Ray2D rays[raysCount];
    RaycastHit hits[raysCount];

    Image img = GenImageColor(textureWidth, textureHeight, BLANK);
    ImageFormat(&img, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
    Texture2D tex = LoadTextureFromImage(img);

    Rectangle source = { 0, 0, (float)textureWidth, (float)textureHeight };
    Rectangle dest = { 600, 0, (float)screenWidth - 600, (float)screenHeight };
    Vector2 origin = { 0, 0 };

    Color *pixels = (Color *)img.data;
    while (!WindowShouldClose()) {
        MovePlayer(&player);
        Raycast(player, rays, hits);
        Draw3D(textureWidth, textureHeight, player, rays, hits, pixels);
        
        UpdateTexture(tex, img.data);

        BeginDrawing();
        ClearBackground(BLACK);
        Draw2D(player, rays, hits);
        DrawTexturePro(tex, source, dest, origin, 0.0f, WHITE);
        DrawFPS(5, 5);
        EndDrawing();

        ImageClearBackground(&img, BLANK);
    }

    UnloadTexture(tex);
    UnloadImage(img);
    CloseWindow();
    return 0;
}
