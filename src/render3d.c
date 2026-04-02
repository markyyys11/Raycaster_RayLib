#include <math.h>
#include <stdlib.h>
#include "raylib.h"
#include "math.h"
#include "render3d.h"
#include "settings.h"
#include "types.h"
#include "map.h"
#include "utils.h"
#include "resources.h"

void DrawWall(const int x, const int wallStart, const int wallHeight, const Ray2D ray, const RaycastHit hit, Color *pixels) {
    // int wall = walls[hit.cell.y * mapWidth + hit.cell.x];
    // if (wall <= 0) return;

    // int pixelCol = 0;
    // if (hit.polar) pixelCol = (int)(hit.position.x * textureSize) - (int)(hit.cell.x * textureSize);
    // else if (!hit.polar) pixelCol = (int)(hit.position.y * textureSize) - (int)(hit.cell.y * textureSize);

    // for (int y = 0; y < wallHeight; ++y) {
    //     if (y + wallStart < 0 || y + wallStart >= renderHeight) continue;
    //     int pixelRow = roundf((y * textureSize) / wallHeight);
    //     Color color = GetAtlasPixel(wall - 1, (Vector2Int){.x = pixelCol, .y = pixelRow});
    //     color = ColorMultiply(color, hit.polar ? LIGHTGRAY : GRAY);
    //     pixels[(y + wallStart) * renderWidth + x] = color;
    // }     
}

static void DrawFloorCeilPixel(const int x, const int y, const int pixelCol, const int pixelRow, const int floorIndex, Color *pixels, bool isCeil, Color tint) {
    Color color = GetAtlasPixel(floorIndex - 1, (Vector2Int){.x = pixelCol, .y = pixelRow});
    color = ColorMultiply(color, tint);
    pixels[((renderHeight / 2) + (isCeil ? -y : y)) * renderWidth + x] = color;
}

void DrawFloorCeil(const int x, const float wallHeight, const float projPlaneDist, const float worldWallHeight, const Player player, const Ray2D ray, Color *pixels) {
    float rayDir = ray.angle - player.angle;
    
    for (int y = wallHeight / 2; y < (renderHeight / 2); y++) {
        float tanA = y / projPlaneDist;
        float rayDist = ((worldWallHeight / 2.0f) / tanA) / cosf(rayDir);
        if (floorf(rayDist) > dof) continue;

        float rayX = player.position.x + rayDist * cosf(ray.angle);
        float rayY = player.position.y + rayDist * sinf(ray.angle);

        if (rayX < 0 || rayX >= mapWidth) continue;
        if (rayY < 0 || rayY >= mapHeight) continue;

        int floorIndex = floors[(int)rayY * mapWidth + (int)rayX];
        if (floorIndex <= 0) continue;
        int ceilIndex = ceils[(int)rayY * mapWidth + (int)rayX];
        if (ceilIndex <= 0) continue;

        int pixelCol = textureSize * (rayX - floorf(rayX));
        if (pixelCol > textureSize) continue;

        int pixelRow = textureSize * (rayY - floorf(rayY));
        if (pixelRow > textureSize) continue;

        DrawFloorCeilPixel(x, y, pixelCol, pixelRow, floorIndex, pixels, false, WHITE);
        DrawFloorCeilPixel(x, y, pixelCol, pixelRow, ceilIndex, pixels, true, LIGHTGRAY);
    }
}

void Draw3D(const Player player, const Ray2D rays[], const RaycastHit hits[], Color *pixels) {
    float worldWallHeight = 1.0f;

    // Projection plane distance
    float projPlaneDist = (renderWidth / 2.0f) / tanf(fov / 2.0f * DEG2RAD);
    float textureHalfHeight = renderHeight / 2.0f;

    for (int i = 0; i < raysCount; i++) {
        Ray2D ray = rays[i];
        RaycastHit hit = hits[i];

        float rayDir = ray.angle - player.angle;
        float rayDist = cosf(rayDir) * hit.distance;

        int wallHeight = roundf((worldWallHeight / rayDist) * projPlaneDist);
        int wallStart = roundf(textureHalfHeight - (wallHeight / 2.0f));

        DrawFloorCeil(i, wallHeight, projPlaneDist, worldWallHeight, player, ray, pixels);
        DrawWall(i, wallStart, wallHeight, ray, hit, pixels);
    }
}