#include <math.h>
#include <stdlib.h>
#include "raylib.h"
#include "math.h"
#include "render_raycast.h"
#include "settings.h"
#include "types.h"
#include "map.h"
#include "utils.h"
#include "resources.h"

const float worldWallHeight = 1.0f;
const float halfWallHeight = worldWallHeight / 2.0f;

void DrawWall2(const int x, const int wallStart, const int wallHeight, const RaycastHit hit, Color *pixels) {
    int wall = walls[hit.cell.y * mapWidth + hit.cell.x];
    if (wall <= 0) return;

    int pixelCol = 0;
    if (hit.polar) pixelCol = (int)(hit.position.x * textureSize) - (int)(hit.cell.x * textureSize);
    else if (!hit.polar) pixelCol = (int)(hit.position.y * textureSize) - (int)(hit.cell.y * textureSize);

    for (int y = 0; y < wallHeight; ++y) {
        if (y + wallStart < 0 || y + wallStart >= renderHeight) continue;
        int pixelRow = (int)((y * textureSize) / wallHeight);
        Color color = GetAtlasPixel(wall - 1, (Vector2Int){.x = pixelCol, .y = pixelRow});
        color = ColorMultiply(color, hit.polar ? LIGHTGRAY : GRAY);
        pixels[(y + wallStart) * renderWidth + x] = color;
    }     
}

static void DrawFloorCeilPixel2(const int x, const int y, const int pixelCol, const int pixelRow, const int floorIndex, Color *pixels, bool isCeil, Color tint) {
    Color color = GetAtlasPixel(floorIndex - 1, (Vector2Int){.x = pixelCol, .y = pixelRow});
    color = ColorMultiply(color, tint);
    pixels[((renderHeight / 2) + (isCeil ? -y : y)) * renderWidth + x] = color;
}

void DrawFloorCeil2(const int x, const float wallHeight, const float projPlaneDist, const float worldWallHeight, const Player player, const Ray2D ray, Color *pixels) {
    float rayDir = ray.angle - player.angle;
    
    for (int y = wallHeight / 2; y < (renderHeight / 2); y++) {
        float tanA = y / projPlaneDist;
        float rayDist = ((worldWallHeight / 2.0f) / tanA) / cosf(rayDir);
        if ((int)(rayDist) > dof) continue;

        float rayX = player.position.x + rayDist * ray.cosAngle;
        float rayY = player.position.y + rayDist * ray.sinAngle;

        if (rayX < 0 || rayX >= mapWidth) continue;
        if (rayY < 0 || rayY >= mapHeight) continue;

        int pixelCol = textureSize * (rayX - (int)(rayX));
        if (pixelCol > textureSize) continue;

        int pixelRow = textureSize * (rayY - (int)(rayY));
        if (pixelRow > textureSize) continue;

        int floorIndex = floors[(int)rayY * mapWidth + (int)rayX];
        if (floorIndex > 0) DrawFloorCeilPixel2(x, y, pixelCol, pixelRow, floorIndex, pixels, false, WHITE);
        int ceilIndex = ceils[(int)rayY * mapWidth + (int)rayX];
        if (ceilIndex > 0) DrawFloorCeilPixel2(x, y, pixelCol, pixelRow, ceilIndex, pixels, true, LIGHTGRAY);
    }
}

void DrawRaycast(const Player player, Color *pixels) {
    float pX = player.position.x, pY = player.position.y;

    // Projection plane distance
    float projPlaneDist = (renderWidth / 2.0f) / tanf(fov / 2.0f * DEG2RAD);
    float renderVertCenter = renderHeight / 2.0f;


    float fovRad = fov * DEG2RAD;
    float startAngle = (player.angle - fovRad / 2);
    float halfWidth = tanf(fovRad * 0.5f);

    for (int i = 0; i < raysCount; i++) {
        // Ray angles
        float rN = (i + 0.5f) / (float)raysCount - 0.5f;
        float xO = rN * 2.0f * halfWidth;
        float rayDeltaAngle = atan2f(xO, 1.0f), rayAngle = player.angle + rayDeltaAngle;
        float rayCos = cosf(rayAngle), raySin = sinf(rayAngle), rayTan = tanf(rayAngle);

        // Horizontal ray start
        float hX = 9999, hY = 9999, dHY = 0, dHX = 0;
        if (raySin > 0) { hY = (int)(pY + 1) - pY; hX = hY / rayTan; dHY = 1; }
        else if (raySin < 0) { hY = -((pY - (int)pY) + 0.00001f); hX = hY / rayTan; dHY = -1; }
        dHX = dHY / rayTan; hY = pY + hY; hX = pX + hX;

        // Vertical ray start
        float vX = 9999, vY = 9999, dVY = 0, dVX = 0;
        if (rayCos > 0) { vX = (int)(pX + 1) - pX; vY = vX * rayTan; dVX = 1; } 
        else if (rayCos < 0) { vX = -((pX - (int)pX) + 0.00001f); vY = vX * rayTan; dVX = -1; }     
        dVY = dVX * rayTan; vX = pX + vX; vY = pY + vY;

        float hCD = 9999, vCD = 9999;
        for (int i = 0; i < dof; i++) {
            
        }

        bool horCollided = false, verCollider = false;
        int dofIter = 0;
        while (dofIter < dof) {
            if (raySin != 0) {
                if (hX > 0 && hX < mapWidth && hY > 0 && hY < mapHeight) {
                    if (walls[(int)(hY) * mapWidth + (int)(hX)] <= 0) {
                        hX += dHX; hY += dHY;
                    } else horCollided = true;
                }
            }

            if (rayCos != 0) {
                if (vX > 0 && vX < mapWidth && vY > 0 && vY < mapHeight) {
                    if (walls[(int)(vY) * mapWidth + (int)(vX)] <= 0) {
                        vX += dVX;
                        vY += dVY;
                    } else verCollider = true;
                }
            }


            dofIter += 1;
        }

        float horLen = 9999, verLen = 9999;
        if (horCollided) {
            float horDX = hX - pX,
                horDY = hY - pY;
            horLen = horDX * horDX + horDY * horDY; 
        }

        if (verCollider) {
            float verDX = vX - pX,
                verDY = vY - pY;
            verLen = verDX * verDX + verDY * verDY;
        }
        
        Vector2 position = {.x = horLen < verLen ? hX : vX, .y = horLen < verLen ? hY : vY};
        Vector2Int cell = {.x = (int)position.x, .y = (int)position.y};

        float rayLength = horLen < verLen ? sqrtf(horLen) : sqrtf(verLen); 
        RaycastHit hit;
        hit.distance = rayLength;       
        hit.cell = cell;
        hit.position = position;
        hit.polar = horLen < verLen ? true : false;

        // float rayDir = rayDeltaAngle;
        float rayDist = cosf(rayDeltaAngle) * hit.distance;

        // int wallProjHeight = (int)((worldWallHeight / rayDist) * projPlaneDist);
        int wallProjHeight = (int)(projPlaneDist / (rayDist / worldWallHeight));
        int projVerticalDelta = (int)(projPlaneDist / (rayDist / player.position.z));
        // float halfWall = wallHeight / 2.0f;
        // float wallStartDelta = player.position.z - halfWall; Подходит для вертикалього поворота камеры

        int wallStart = (int)(renderVertCenter + projVerticalDelta - wallProjHeight / 2);

        // DrawFloorCeil2(i, wallProjHeight, projPlaneDist, worldWallHeight, player, ray, pixels);
        DrawWall2(i, wallStart, wallProjHeight, hit, pixels);
    }
}