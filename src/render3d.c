#include <math.h>
#include "raylib.h"
#include "math.h"
#include "render3d.h"
#include "settings.h"
#include "types.h"
#include "map.h"
#include "utils.h"

void Draw3D(const int screenWidth, const int screenHeight, const Player player, const Ray2D rays[], const RaycastHit hits[], Color *pixels) {
    int halfScreenHeight = screenHeight / 2;
    float maxDim = (mapHeight > mapWidth) ? mapHeight : mapWidth;
    float fovRad = fov * DEG2RAD;
    float k = (screenHeight * maxDim) / (tanf(fovRad / 2.0f));
    float columnWidth = (float)screenWidth / (float)raysCount;

    for (int i = 0; i < raysCount; ++i) {
        float angleDiff = rays[i].angle - player.angle;
        float perpDist = hits[i].distance * fabsf(cosf(angleDiff));
        if (perpDist < 0.0001f) perpDist = 0.0001f;
        int wallHeight = (int)((k / perpDist) * wallHeightMultiplier);
        if (wallHeight > screenHeight) wallHeight = screenHeight;
        if (wallHeight < 0) wallHeight = 0;
        int wallStart = halfScreenHeight - wallHeight / 2;
        int wallWidth = screenWidth / raysCount;
        
        for (int w = 0; w < wallWidth; ++w) {
            for (int y = wallStart; y < wallStart + wallHeight; ++y) {
                int x = i * wallWidth + w;
                if (y >= 0 && y < screenHeight) {
                    Color color = ColorMultiply(wallColors[walls[hits[i].cell.y * mapWidth + hits[i].cell.x] - 1], hits[i].polar ? WHITE : GRAY) ;
                    pixels[y * screenWidth + x] = color;
                }
            }
        }
    }
}