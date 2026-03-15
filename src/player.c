#include <math.h>
#include "raylib.h"
#include "types.h"
#include "player.h"
#include "settings.h"

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

    if (IsKeyDown(KEY_RIGHT)) player->angle += cameraSpeed * deltaTime;
    else if (IsKeyDown(KEY_LEFT)) player->angle -= cameraSpeed * deltaTime;
}
