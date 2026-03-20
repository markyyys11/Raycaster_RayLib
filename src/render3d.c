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
    int wall = walls[hit.cell.y * mapWidth + hit.cell.x];
    if (wall <= 0) return;

    int pixelCol = 0;
    if (hit.polar) pixelCol = textureSize * (hit.position.x - hit.cell.x);
    else if (!hit.polar) pixelCol = textureSize * (hit.position.y - hit.cell.y);

    for (int y = 0; y < wallHeight; ++y) {
        if (y + wallStart < 0 || y + wallStart >= textureHeight) continue;
        int pixelRow = (y * textureSize) / wallHeight;
        Color color = GetAtlasPixel(wall - 1, (Vector2Int){.x = pixelCol, .y = pixelRow});
        color = ColorMultiply(color, hit.polar ? WHITE : LIGHTGRAY);
        pixels[(y + wallStart) * textureWidth + x] = color;
    }     
}

void DrawFloor(const int x, const Player player, const int wallBottom, const rayDir, const Ray2D ray, const float rayDist, Color *pixels) {
    for (int y = 0; y < textureHeight - wallBottom; y++) {
        // Vector2 cell = {.x = player.position.x + cosf(ray.angle) * (y * floorDelta / 100), .y = player.position.y + sinf(ray.angle) * (y * floorDelta / 100)};



        if (cell.x < 0 || cell.x >= mapWidth) continue;
        if (cell.y < 0 || cell.y >= mapHeight) continue;

        int textureInd = floors[(int)cell.y * mapWidth + (int)cell.x];

        int pixelCol = (cell.x - floorf(cell.x)) * textureSize;
        int pixelRow = (cell.y - floorf(cell.y)) * textureSize;

        pixels[(textureHeight - (y + 1)) * textureWidth + x] = GetAtlasPixel(textureInd - 1, (Vector2Int){.x = pixelCol, .y = pixelRow});
    }
}

void Draw3D(const Player player, const Ray2D rays[], const RaycastHit hits[], Color *pixels) {
    // Vertical FOV
    float aspectRatio = (float)screenWidth / (float)screenHeight;
    float verticalFov = 2.0f * atanf(tanf(fov / 2.0f * DEG2RAD) / aspectRatio) * RAD2DEG;
    
    // Projection plane distance
    // float projPlaneDist = (screenHeight / 2.0f) / tanf(verticalFov / 2.0f * DEG2RAD);
    float projPlaneDist = (screenWidth / 2.0f) / tanf(fov / 2.0f * DEG2RAD);
    float worldWallHeight = 1.0f;

    for (int i = 0; i < raysCount; i++) {
        Ray2D ray = rays[i];
        RaycastHit hit = hits[i];

        float rayDir = ray.angle - player.angle;
        float rayDist = cosf(rayDir) * hit.distance;

        float wallHeight = (worldWallHeight / rayDist) * projPlaneDist;
        float wallStart = (textureHeight / 2) - wallHeight / 2;

        DrawWall(i, wallStart, wallHeight, ray, hit, pixels);
    }
}