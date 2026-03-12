#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "raylib.h"

#include "types.h"
#include "render.h"
#include "settings.h"

Wall *walls = NULL;
int wallsCount = 0;

void InitWalls(int count) {
    wallsCount = count;
    walls = calloc(count, sizeof(Wall));

    int screenWidth = 800;
    int screenHeight = 600;

    for (int i = 0; i < count; ++i) {
        walls[i].start.x = (float)GetRandomValue(0, screenWidth);
        walls[i].start.y = (float)GetRandomValue(0, screenHeight);
        walls[i].end.x = (float)GetRandomValue(0, screenWidth);
        walls[i].end.y = (float)GetRandomValue(0, screenHeight);
    }
}

void FreeWalls(void) {
    if (walls != NULL) {
        free(walls);
        walls = NULL;
        wallsCount = 0;
    }
}

static float DistanceBetween(Vector2 from, Vector2 to) {
    return sqrtf(powf(from.x - to.x, 2) + powf(from.y - to.y, 2));
}

void DrawWalls(Player player) {
    float fov = 50.0f;
    float fovRad = fov * DEG2RAD;
    float startAngle = (player.cameraDegree - fov / 2) * DEG2RAD;

    PlayerRay *rays = malloc(sizeof(PlayerRay) * raysCount);
    
    for (int i = 0; i < raysCount; ++i) {
        float factor = (float)i / ((float)raysCount - 1.0f);
        float angle = startAngle + factor * fovRad;

        rays[i].angle = angle;
        rays[i].angleDiff = angle - (player.cameraDegree * DEG2RAD);
        rays[i].direction.x = cosf(angle);
        rays[i].direction.y = sinf(angle);
    }

    float halfScreenHeight = (float)screenHeight / 2.0f;
    int wallWidth = screenWidth / raysCount;
    float projectionPlaneDist = halfScreenHeight * 20.0f / tanf(fovRad / 2.0f);

    for (int i = 0; i < raysCount; ++i) {
        Vector2 lookAt;
        lookAt.x = player.position.x + rays[i].direction.x * 1000.0f;
        lookAt.y = player.position.y + rays[i].direction.y * 1000.0f;

        float maxDist = DistanceBetween(player.position, lookAt);
        float nearestDist = maxDist;
        
        for (int j = 0; j < wallsCount; ++j) {
            Vector2 intersection;
            if (CheckCollisionLines(player.position, lookAt, walls[j].start, walls[j].end, &intersection)) {
                float dist = DistanceBetween(player.position, intersection);
                if (dist < nearestDist) {
                    lookAt = intersection;
                    nearestDist = dist;
                }
            }
        }
        
        float correctedDist = nearestDist * cosf(rays[i].angleDiff);
        if (correctedDist < 0.01f) correctedDist = 0.01f;

        float wallHalf = projectionPlaneDist / correctedDist;

        float gray = 255.0f - fminf(nearestDist * 0.5f, 255.0f);
        Color c = { (unsigned char)gray, (unsigned char)gray, (unsigned char)gray, 255 };

        for (int j = 0; j < wallWidth; ++j) {
            float x = (float)((i * wallWidth) + j);
            Vector2 top = { x, halfScreenHeight - wallHalf };
            Vector2 bottom = { x, halfScreenHeight + wallHalf };
            DrawLineV(top, bottom, c);
        }
    }

    free(rays);
}
