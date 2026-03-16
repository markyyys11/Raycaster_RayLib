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

Color *tex;
void LoadTextureBuffer() {
    tex = malloc(16 * 16 * sizeof(Color));
}

void DrawWalls(const int screenWidth, const int screenHeight, const Player player, const Ray2D rays[], const RaycastHit hits[], Color *pixels) {
    int halfScreenHeight = screenHeight / 2;
    // float maxDim = (mapHeight > mapWidth) ? mapHeight : mapWidth;
    // float fovRad = fov * DEG2RAD;
    // float k = (screenHeight * maxDim) / (tanf(fovRad / 2.0f));
    // float columnWidth = (float)screenWidth / (float)raysCount;

    float projDist = 0.5f / tanf(((fov * DEG2RAD) * screenHeight / screenWidth) / 2);
    for (int i = 0; i < raysCount; ++i) {
        float rayDir = rays[i].angle - player.angle;
        float projPos = (tanf(rayDir) / 2) / tanf(fov / 2);
        float wallHeight = roundf((screenHeight * projDist) / (hits[i].distance * cosf(rayDir)));
        int wallStart = halfScreenHeight - wallHeight / 2;

        Cell cell = hits[i].cell;
        int wall = walls[hits[i].cell.y * mapWidth + hits[i].cell.x];

        if (wall <= 0) continue;
        GetTexture(wall - 1, 16, 16, tex);

        int pixelCol = 0;
        if (hits[i].polar) pixelCol = 16 * (hits[i].position.x - hits[i].cell.x);
        else if (!hits[i].polar) pixelCol = 16 * (hits[i].position.y - hits[i].cell.y);

        for (int y = 0; y < wallHeight; ++y) {
            if (y + wallStart < 0 || y + wallStart >= screenHeight) continue;
            int pixelRow = (y * 16) / wallHeight;
            Color color = tex[pixelCol * 16 + pixelRow];
            color = ColorMultiply(color, hits[i].polar ? WHITE : LIGHTGRAY);

            int x = i;
            
            pixels[(y + wallStart) * screenWidth + x] = color;
        }

        // float angleDiff = rays[i].angle - player.angle;
        // float perpDist = hits[i].distance * fabsf(cosf(angleDiff));
        // if (perpDist < 0.0001f) perpDist = 0.0001f;
        // int wallHeight = (int)((k / perpDist) * wallHeightMultiplier);
        // int wallStart = halfScreenHeight - wallHeight / 2;
        // int wallWidth = screenWidth / raysCount;
        
        // Cell cell = hits[i].cell;
        // int wall = walls[hits[i].cell.y * mapWidth + hits[i].cell.x];

        // if (wall <= 0) continue;
        // GetTexture(wall - 1, 16, 16, tex);

        // int pixelCol = 0;
        // if (hits[i].polar) pixelCol = 16 * (hits[i].position.x - hits[i].cell.x);
        // else if (!hits[i].polar) pixelCol = 16 * (hits[i].position.y - hits[i].cell.y);

        // for (int w = 0; w < wallWidth; ++w) {
        //     for (int y = 0; y < wallHeight; ++y) {
        //          if (y + wallStart < 0 || y + wallStart >= screenHeight) continue;
        //         int pixelRow = (y * 16) / wallHeight;
        //         Color color = tex[pixelCol * 16 + pixelRow];
        //         color = ColorMultiply(color, hits[i].polar ? WHITE : LIGHTGRAY);

        //         int x = i * wallWidth + w;
               
        //         pixels[(y + wallStart) * screenWidth + x] = color;
        //     }
        // }   
    }
}

void Draw3D(const int screenWidth, const int screenHeight, const Player player, const Ray2D rays[], const RaycastHit hits[], Color *pixels) {
   DrawWalls(screenWidth, screenHeight, player, rays, hits, pixels); 
}

void UnloadTextureBuffer() {
    free(tex);
}