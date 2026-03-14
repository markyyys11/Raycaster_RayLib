#include "raycast.h"
#include "math.h"
#include "settings.h"
#include "map.h"

void InitRays(const Player player, Ray2D rays[raysCount]) {
    float fovRad = fov * DEG2RAD;
    float startAngle = (player.angle - fovRad / 2);

    for (int i = 0; i < raysCount; ++i) {
        float factor = (float)i / (raysCount > 1? (float)raysCount : 2.0f - 1.0f);
        float angle = startAngle + factor * fovRad;

        rays[i].angle = angle;
        rays[i].cosAngle = cosf(angle);
        rays[i].sinAngle = sinf(angle);
    }
}

void Raycast(const Player player, Ray2D rays[raysCount], RaycastHit hits[raysCount]) {
    InitRays(player, rays);
    
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

        hits[i].distance = horLen < verLen ? horLen : verLen;        
        hits[i].cell = cell;
        hits[i].polar = horLen < verLen ? true : false;
    }   
}