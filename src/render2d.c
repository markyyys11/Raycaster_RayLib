#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "raylib.h"
#include "raymath.h"
#include "render2d.h"

#include "player.h"
#include "types.h"
#include "settings.h"
#include "map.h"

Ray2D *rays;

void InitRays() {
    float fovRad = fov * DEG2RAD;
    float startAngle = (player.angle - fovRad / 2);

    rays = malloc(sizeof(Ray2D) * raysCount);
    
    for (int i = 0; i < raysCount; ++i) {
        float factor = (float)i / (raysCount > 1? (float)raysCount : 2.0f - 1.0f);
        float angle = startAngle + factor * fovRad;

        rays[i].angle = angle;
        rays[i].cosAngle = cosf(angle);
        rays[i].sinAngle = sinf(angle);
    }
}

void Draw2D() {
    for (int x = 0; x < mapWidth; x++) {
        for (int y = 0; y < mapHeight; y++) {
            if (walls[y * mapHeight + x]) DrawRectangle(x * tileSize + 1, y * tileSize + 1, tileSize - 2, tileSize - 2,  WHITE);
            else DrawRectangle(x * tileSize + 1, y * tileSize + 1, tileSize - 2, tileSize - 2,  GRAY);
        }
    }

    DrawCircle(player.position.x, player.position.y, 5, RED);

    InitRays();
    float xo = player.position.x, yo = player.position.y;
    for (int i = 0; i < raysCount; i++) {
        DrawLine(
            xo, yo, 
            xo + rays[i].cosAngle * 20, yo + rays[i].sinAngle * 20, RED);

        float xdo = sqrt(tileSize + powf(rays[i].sinAngle / rays[i].cosAngle, 2)),
              ydo = sqrt(tileSize + powf(rays[i].cosAngle / rays[i].sinAngle, 2));

        DrawCircle(xo + xdo, yo + ydo, 5, RED);
        
    }

    free(rays);
}