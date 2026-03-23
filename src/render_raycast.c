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

void DrawWall2(const int x, const int wallStart, const int wallHeight, const Ray2D ray, const RaycastHit hit, Color *pixels) {
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
    // Projection plane distance
    float projPlaneDist = (renderWidth / 2.0f) / tanf(fov / 2.0f * DEG2RAD);
    float textureHalfHeight = renderHeight / 2.0f;

    float fovRad = fov * DEG2RAD;
    float startAngle = (player.angle - fovRad / 2);
    float halfWidth = tanf(fovRad * 0.5f);

    for (int i = 0; i < raysCount; i++) {
        float t = (i + 0.5f) / (float)raysCount - 0.5f;
        float xOffset = t * 2.0f * halfWidth;
        float angleOffset = atan2f(xOffset, 1.0f);

        Ray2D ray;// = rays[i];
        ray.angle = player.angle + angleOffset;
        ray.cosAngle = cosf(ray.angle);
        ray.sinAngle = sinf(ray.angle);
        ray.tanAngle = tanf(ray.angle);

        float horX = 0, horY = 0, deltaHorY = 0, deltaHorX = 0;
        
        if (ray.sinAngle > 0) {
            horY = (int)(player.position.y + 1) - player.position.y;
            horX = horY / ray.tanAngle;
            deltaHorY = 1;
        }
        else if (ray.sinAngle < 0) {
            horY = -((player.position.y - (int)(player.position.y)) + 0.00001f);
            horX = horY / ray.tanAngle;
            deltaHorY = -1;
        } else {
            horY = 9999;
            horX = 9999;
        }

        deltaHorX = deltaHorY / ray.tanAngle;
        horY = player.position.y + horY;
        horX = player.position.x + horX; 

        float verX = 0, verY = 0, deltaVerY = 0, deltaVerX = 0;

        if (ray.cosAngle > 0) {
            verX = (int)(player.position.x + 1) - player.position.x;
            verY = verX * ray.tanAngle;
            deltaVerX = 1; 
        } else if (ray.cosAngle < 0) {
            verX = -((player.position.x - (int)(player.position.x)) + 0.00001f);
            verY = verX * ray.tanAngle;
            deltaVerX = -1;
        } else {
            verX = 9999; 
            verY = 9999; 
        }

        deltaVerY = deltaVerX * ray.tanAngle;
        verX = player.position.x + verX;
        verY = player.position.y + verY;

        bool horCollided = false, verCollider = false;

        int dofIter = 0;
        while (dofIter < dof) {
            if (ray.sinAngle != 0) {
                if (horX > 0 && horX < mapWidth && horY > 0 && horY < mapHeight) {
                    if (walls[(int)(horY) * mapWidth + (int)(horX)] <= 0) {
                        horX += deltaHorX;
                        horY += deltaHorY;
                    } else horCollided = true;
                }
            }

            if (ray.cosAngle != 0) {
                if (verX > 0 && verX < mapWidth && verY > 0 && verY < mapHeight) {
                    if (walls[(int)(verY) * mapWidth + (int)(verX)] <= 0) {
                        verX += deltaVerX;
                        verY += deltaVerY;
                    } else verCollider = true;
                }
            }


            dofIter += 1;
        }

        float horLen = 9999, verLen = 9999;
        if (horCollided) {
            float horDX = horX - player.position.x,
                horDY = horY - player.position.y;
            horLen = sqrtf(horDX * horDX + horDY * horDY); 
        }

        if (verCollider) {
            float verDX = verX - player.position.x,
                verDY = verY - player.position.y;
            verLen = sqrtf(verDX * verDX + verDY * verDY);
        }
        
        Vector2 position = {.x = horLen < verLen ? horX : verX, .y = horLen < verLen ? horY : verY};
        Vector2Int cell = {.x = (int)position.x, .y = (int)position.y};

        float rayLength = horLen < verLen ? horLen : verLen; 
        RaycastHit hit;
        hit.distance = rayLength;       
        hit.cell = cell;
        hit.position = position;
        hit.polar = horLen < verLen ? true : false;

        float rayDir = ray.angle - player.angle;
        float rayDist = cosf(rayDir) * hit.distance;

        int wallHeight = (int)((worldWallHeight / rayDist) * projPlaneDist);
        int wallStart = (int)(textureHalfHeight - (wallHeight / 2.0f));

        DrawFloorCeil2(i, wallHeight, projPlaneDist, worldWallHeight, player, ray, pixels);
        DrawWall2(i, wallStart, wallHeight, ray, hit, pixels);
    }
}