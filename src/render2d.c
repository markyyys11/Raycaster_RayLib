#include "render2d.h"
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "raylib.h"
#include "raymath.h"
#include "types.h"
#include "settings.h"
#include "map.h"
#include "utils.h"

void Draw2D(const Player player, const Ray2D rays[raysCount], const RaycastHit hits[raysCount]) {
    for (int x = 0; x < mapWidth; x++) {
        for (int y = 0; y < mapHeight; y++) {
            if (walls[y * mapWidth + x]) DrawRectangle(x * tileSize + 1, y * tileSize + 1, tileSize - 2, tileSize - 2,  WHITE);
            else DrawRectangle(x * tileSize + 1, y * tileSize + 1, tileSize - 2, tileSize - 2,  LIGHTGRAY);
        }
    }

    char buffer[50];
    snprintf(buffer, sizeof(buffer), "x: %.2f, y: %.2f", player.position.x, player.position.y);
    DrawText(buffer, 10, screenHeight - 24, 14, RED);

    DrawCircle(player.position.x * tileSize, player.position.y * tileSize, 5, RED);

    for (int i = 0; i < raysCount; i++) {
        Color color = ColorMultiply(wallColors[walls[hits[i].cell.y * mapWidth + hits[i].cell.x] - 1], hits[i].polar ? WHITE : GRAY) ;
        DrawLine(player.position.x * tileSize, player.position.y * tileSize, 
                (player.position.x + hits[i].distance * rays[i].cosAngle) * tileSize, 
                (player.position.y + hits[i].distance * rays[i].sinAngle) * tileSize, color);

        DrawCircle(hits[i].cell.x * tileSize, hits[i].cell.y * tileSize, 5, RED);
    }
}

