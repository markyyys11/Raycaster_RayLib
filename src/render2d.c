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
RaycastHit *rayDists;

void InitRays() {
    float fovRad = fov * DEG2RAD;
    float startAngle = (player.angle - fovRad / 2);

    if (sizeof(rays) > 0) MemFree(rays);
    rays = malloc(sizeof(Ray2D) * raysCount);
    rayDists = malloc(sizeof(RaycastHit) * raysCount);
    
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

void CalcRayDistance() {
    InitRays();
    for (int i = 0; i < raysCount; i++) {
        Ray2D ray = rays[i];
        float horX = 0, horY = 0, deltaHorY = 0, deltaHorX = 0;
        
        if (ray.sinAngle > 0) {
            horY = floor(player.position.y + 1) - player.position.y;
            horX = horY / tanf(ray.angle);
            deltaHorY = 1;
        }
        else if (ray.sinAngle < 0) {
            horY = -((player.position.y - floor(player.position.y)) + 0.001f);
            horX = horY / tanf(ray.angle);
            deltaHorY = -1;
        } else {
            horY = 9999;
            horX = 9999;
        }

        deltaHorX = deltaHorY / tanf(ray.angle);
        horY = player.position.y + horY;
        horX = player.position.x + horX; 

        float verX = 0, verY = 0, deltaVerY = 0, deltaVerX = 0;

        if (ray.cosAngle > 0) {
            verX = floor(player.position.x + 1) - player.position.x;
            verY = verX * tanf(ray.angle);
            deltaVerX = 1; 
        } else if (ray.cosAngle < 0) {
            verX = -((player.position.x - floor(player.position.x)) + 0.001f);
            verY = verX * tanf(ray.angle);
            deltaVerX = -1;
        } else {
            verX = 9999; 
            verY = 9999; 
        }

        deltaVerY = deltaVerX * tanf(ray.angle);
        verX = player.position.x + verX;
        verY = player.position.y + verY;

        bool horCollided = false, verCollider = false;

        int dofIter = 0;
        while (dofIter < dof) {
            if (ray.sinAngle == 0) break;
            if (horX < 0 || horX > mapWidth || horY < 0 || horY > mapHeight) break;
            if (walls[(int)(horY) * mapHeight + (int)(horX)] > 0) break;

            horX += deltaHorX;
            horY += deltaHorY;

            dofIter += 1;
        }
         
        dofIter = 0;
        while (dofIter < dof) {
            if (ray.cosAngle == 0)  break;
            if (verX < 0 || verX > mapWidth || verY < 0 || verY > mapHeight) break;
            if (walls[(int)(verY) * mapHeight + (int)(verX)] > 0) break;

            verX += deltaVerX;
            verY += deltaVerY;

            dofIter += 1;
        }

        float horLen = 9999, verLen = 9999;
        horLen = sqrtf(powf(horX - player.position.x, 2) + powf(horY - player.position.y, 2)); 
        verLen = sqrtf(powf(verX - player.position.x, 2) + powf(verY - player.position.y, 2));
        Cell cell = {.x = (int)horLen < verLen ? horX : verX, .y = (int)horLen < verLen ? horY : verY};

        rayDists[i].distance = horLen < verLen ? horLen : verLen;        
        rayDists[i].cell = cell;
        rayDists[i].polar = horLen < verLen ? true : false;
    }   
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

    CalcRayDistance();
    for (int i = 0; i < raysCount; i++) {
        DrawLine(player.position.x * tileSize, player.position.y * tileSize, 
                (player.position.x + rayDists[i].distance * rays[i].cosAngle) * tileSize, 
                (player.position.y + rayDists[i].distance * rays[i].sinAngle) * tileSize, rayDists[i].polar ? GREEN : YELLOW);
    }
}

