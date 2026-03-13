#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>

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

    if (sizeof(rays) > 0) MemFree(rays);
    rays = malloc(sizeof(Ray2D) * raysCount);
    
    for (int i = 0; i < raysCount; ++i) {
        float factor = (float)i / (raysCount > 1? (float)raysCount : 2.0f - 1.0f);
        float angle = startAngle + factor * fovRad;

        rays[i].angle = angle;
        rays[i].cosAngle = cosf(angle);
        rays[i].sinAngle = sinf(angle);
    }
}

Vector2 player2D() {
    Vector2 pos = {.x = player.position.x * tileSize, .y = player.position.y * tileSize};
    return pos;
}

void Draw2D() {
    for (int x = 0; x < mapWidth; x++) {
        for (int y = 0; y < mapHeight; y++) {
            if (walls[y * mapHeight + x]) DrawRectangle(x * tileSize + 1, y * tileSize + 1, tileSize - 2, tileSize - 2,  WHITE);
            else DrawRectangle(x * tileSize + 1, y * tileSize + 1, tileSize - 2, tileSize - 2,  GRAY);
        }
    }

    char buffer[50];
    snprintf(buffer, sizeof(buffer), "x: %.2f, y: %.2f", player.position.x, player.position.y);
    DrawText(buffer, 10, screenHeight - 24, 14, RED);

    DrawCircle(player2D().x, player2D().y, 5, RED);

    InitRays();
    for (int i = 0; i < raysCount; i++) {
        DrawLine(
            player2D().x, player2D().y, 
            player2D().x + rays[i].cosAngle * 20, player2D().y + rays[i].sinAngle * 20, RED);
    }

    for (int i = 0; i < raysCount; i++) {
        Ray2D ray = rays[i];
        float deltaHorX = 0, deltaHorY = 0;
        
        if (ray.sinAngle > 0) {
            deltaHorY = floor(player.position.y + 1) - player.position.y;
            deltaHorX = deltaHorY / tanf(ray.angle);
        } else if (ray.sinAngle < 0) {
            deltaHorY = -(player.position.y - floor(player.position.y) + 0.001f);
            deltaHorX = deltaHorY / tanf(ray.angle);
        }

        float cacheHorX = tileSize / tanf(ray.angle), cacheHorY = tileSize;

        int dofIter = 0;
        while (dofIter < dof) {
            if (player.position.x + deltaHorX >= mapWidth || player.position.y + deltaHorY >= mapHeight)
                break;

            if (walls[(int)(player.position.y + deltaHorY) * mapHeight + (int)(player.position.x + deltaHorX)] > 0)
                break;
            
            DrawCircle((player.position.x + deltaHorX) * tileSize, (player.position.y + deltaHorY) * tileSize, 3, GREEN);
            dofIter++;
        }
    }
    

    // for (int i = 0; i < raysCount; i++) {
    //     Ray2D ray = rays[i];
    //     // Horizontal lines
    //     float horX = px, horY = py;
    //     int cx = pcx, cy = pcy;
    //     bool horCollided = false;
    //     if (ray.sinAngle < 0) {
    //         horX -= (py - (pcy * tileSize)) / tanf(ray.angle);
    //         horY = ((pcy - 1) * tileSize) + tileSize - 0.001f;
    //         cx = floor(horX / tileSize);
    //         cy = floor(horY / tileSize);
    //     }

    //     DrawCircle(horX, horY, 2, GREEN);
    // }
}