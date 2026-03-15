#include <math.h>
#include "raylib.h"
#include "types.h"
#include "player.h"
#include "settings.h"
#include "map.h"

void MovePlayer(Player *player) {
    float deltaTime = GetFrameTime();
    if (IsKeyDown(KEY_W)) {
        player->position.x += (moveSpeed * cosf(player->angle)) * deltaTime;
        player->position.y += moveSpeed * sinf(player->angle) * deltaTime;
    }
    else if (IsKeyDown(KEY_S)) {
        player->position.x -= moveSpeed * cosf(player->angle) * deltaTime;
        player->position.y -= moveSpeed * sinf(player->angle) * deltaTime;
    }

    if (IsKeyDown(KEY_D)) {
        player->position.x += moveSpeed * cosf(player->angle + (90 * DEG2RAD)) * deltaTime;
        player->position.y += moveSpeed * sinf(player->angle + (90 * DEG2RAD)) * deltaTime;
    }
    else if (IsKeyDown(KEY_A)) {
        player->position.x -= moveSpeed * cosf(player->angle + (90 * DEG2RAD)) * deltaTime;
        player->position.y -= moveSpeed * sinf(player->angle + (90 * DEG2RAD)) * deltaTime;
    }

    // Vector2 collPointFront = {
    //     player->position.x + (cosf(player->angle) * collisionDistance),
    //     player->position.y + (sinf(player->angle) * collisionDistance)
    // };

    // Vector2 collPointBack = {
    //     player->position.x - (cosf(player->angle) * collisionDistance),
    //     player->position.y - (sinf(player->angle) * collisionDistance)
    // };

    // Vector2 collPointRight = {
    //     player->position.x + (cosf(player->angle + (90 * DEG2RAD)) * collisionDistance),
    //     player->position.y + (sinf(player->angle + (90 * DEG2RAD)) * collisionDistance)
    // };

    // Vector2 collPointLeft = {
    //     player->position.x - (cosf(player->angle + (90 * DEG2RAD)) * collisionDistance),
    //     player->position.y - (sinf(player->angle + (90 * DEG2RAD)) * collisionDistance)
    // };

    // if (walls[(int)(collPointFront.y * mapWidth + collPointFront.x)] > 0) {
    //     player->position.x += collPointFront.x - (int)collPointFront.x;
    //     player->position.y -= collPointFront.y - (int)collPointFront.y;
    // }

    // DrawCircleV(collPointFront, 5, RED);

    if (IsKeyDown(KEY_RIGHT)) player->angle += cameraSpeed * deltaTime;
    else if (IsKeyDown(KEY_LEFT)) player->angle -= cameraSpeed * deltaTime;
}
