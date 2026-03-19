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
        if (y + wallStart < 0 || y + wallStart >= screenHeight) continue;
        int pixelRow = (y * textureSize) / wallHeight;
        Color color = GetAtlasPixel(wall - 1, (Vector2Int){.x = pixelCol, .y = pixelRow});
        color = ColorMultiply(color, hit.polar ? WHITE : LIGHTGRAY);
        pixels[(y + wallStart) * textureWidth + x] = color;
    }     
}

void DrawFloor(const int x, const int wallHeight, const int wallBottom, const Ray2D ray, const float rayDist, Color *pixels) {
    float angleVert = (fov * textureHeight) / textureWidth;
    float halfWall = 0.5f;
    float projDist = fabs(halfWall / tanf(angleVert * DEG2RAD));
    float floorDist = rayDist - projDist;

    int floorScreen = textureHeight - wallBottom;
    float floorDelta = floorDist / floorScreen;

    for (int y = 0; y < floorScreen; y++) {
        if ((int)(y * floorDelta) % 2 == 0)
            pixels[(y + wallBottom) * textureWidth + x] = RED;
        else   
            pixels[(y + wallBottom) * textureWidth + x] = BLUE;
    }
}

void Draw3D(const Player player, const Ray2D rays[], const RaycastHit hits[], Color *pixels) {
    int halfScreenHeight = textureHeight / 2;
    for (int i = 0; i < raysCount; i++) {
        Ray2D ray = rays[i];
        RaycastHit hit = hits[i];

        float rayDir = ray.angle - player.angle;
        float rayDist = cosf(rayDir) * hit.distance;
        float wallHeight = roundf((textureHeight / rayDist) * wallHeightMultiplier) ;
        float wallStart = halfScreenHeight - wallHeight / 2;

        DrawWall(i, wallStart, wallHeight, ray, hit, pixels);
        DrawFloor(i, wallHeight, wallStart + wallHeight, ray, rayDist, pixels);
    }
}