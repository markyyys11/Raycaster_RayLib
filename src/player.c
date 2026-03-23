#include <math.h>
#include "raylib.h"
#include "types.h"
#include "player.h"
#include "settings.h"
#include "map.h"

void MovePlayer(Player *player) {
    float deltaTime = GetFrameTime();

    float upDownX = moveSpeed * cosf(player->angle) * deltaTime,
        upDownY = moveSpeed * sinf(player->angle) * deltaTime,
        leftRightX = moveSpeed * cosf(player->angle + (90 * DEG2RAD)) * deltaTime,
        leftRightY = moveSpeed * sinf(player->angle + (90 * DEG2RAD)) * deltaTime;

    Vector2Int collFront = {
        player->position.x + (cosf(player->angle) * collisionDistance),
        player->position.y + (sinf(player->angle) * collisionDistance)
    };

    Vector2Int collBack = {
        player->position.x - (cosf(player->angle) * collisionDistance),
        player->position.y - (sinf(player->angle) * collisionDistance)
    };

    Vector2Int collRight = {
        player->position.x + (cosf(player->angle + (90 * DEG2RAD)) * collisionDistance),
        player->position.y + (sinf(player->angle + (90 * DEG2RAD)) * collisionDistance)
    };

    Vector2Int collLeft = {
        player->position.x - (cosf(player->angle + (90 * DEG2RAD)) * collisionDistance),
        player->position.y - (sinf(player->angle + (90 * DEG2RAD)) * collisionDistance)
    };

    if (IsKeyDown(KEY_W)) {
        player->position.x += upDownX;
        player->position.y += upDownY;
    } else if (IsKeyDown(KEY_S)) {
        player->position.x -= upDownX;
        player->position.y -= upDownY;
    }

    if (IsKeyDown(KEY_D)) {
        player->position.x += leftRightX;
        player->position.y += leftRightY;
    } else if (IsKeyDown(KEY_A)) {
        player->position.x -= leftRightX;
        player->position.y -= leftRightY;
    }

    DrawCircle(player->position.x * tileSize, player->position.y * tileSize, 5, RED);
    

    // DrawCircle(collPointFront.x * tileSize, collPointFront.y * tileSize, 5, RED);

    if (IsKeyDown(KEY_RIGHT)) player->angle += cameraSpeed * deltaTime;
    else if (IsKeyDown(KEY_LEFT)) player->angle -= cameraSpeed * deltaTime;
}


