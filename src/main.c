#include "raylib.h"
#include "types.h"
#include "player.h"
#include "settings.h"
#include "render2d.h"
#include "render3d.h"
#include "raycast.h"
#include "resources.h"

int main(void) {
    InitWindow(screenWidth, screenHeight, "Raylib Project");

    LoadResources();

    Player player = {.position.x = 5.0f, .position.y = 5.0f, .angle = 0};
    Ray2D rays[raysCount];
    RaycastHit hits[raysCount];

    Image img = GenImageColor(textureWidth, textureHeight, BLANK);
    ImageFormat(&img, PIXELFORMAT_UNCOMPRESSED_R8G8B8A8);
    Texture2D tex = LoadTextureFromImage(img);

    Rectangle source = { 0, 0, (float)textureWidth, (float)textureHeight };
    Rectangle dest = { 0, 0, (float)screenWidth, (float)screenHeight };
    Vector2 origin = { 0, 0 };

    Color *pixels = (Color *)img.data;

    LoadTextureBuffer();
    while (!WindowShouldClose()) {
        BeginDrawing();
            MovePlayer(&player);
            Raycast(player, rays, hits);
            Draw3D(textureWidth, textureHeight, player, rays, hits, pixels);
            UpdateTexture(tex, img.data);
            // ClearBackground(BLACK);
            // Draw2D(player, rays, hits);
            DrawTexturePro(tex, source, dest, origin, 0.0f, WHITE);
            DrawFPS(5, 5);
        EndDrawing();

        ImageClearBackground(&img, BLACK);
    }
    UnloadTextureBuffer();

    UnloadResources();

    UnloadTexture(tex);
    UnloadImage(img);
    CloseWindow();
    return 0;
}
