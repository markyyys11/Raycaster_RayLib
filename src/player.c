#include <math.h>
#include "raylib.h"
#include "types.h"
#include "player.h"
#include "settings.h"

Player player = { .position = { 400.0f, 300.0f }, .angle = 0.0f };


void MovePlayer() {
    if (IsKeyDown(KEY_W)) {
        player.position.x += moveSpeed * cosf(player.angle);
        player.position.y += moveSpeed * sinf(player.angle);
    }
    else if (IsKeyDown(KEY_S)) {
        player.position.x -= moveSpeed * cosf(player.angle);
        player.position.y -= moveSpeed * sinf(player.angle);
    }

    if (IsKeyDown(KEY_D)) {
        player.position.x += moveSpeed * cosf(player.angle + (90 * DEG2RAD));
        player.position.y += moveSpeed * sinf(player.angle + (90 * DEG2RAD));
    }
    else if (IsKeyDown(KEY_A)) {
        player.position.x -= moveSpeed * cosf(player.angle + (90 * DEG2RAD));
        player.position.y -= moveSpeed * sinf(player.angle + (90 * DEG2RAD));
    }

    if (IsKeyDown(KEY_RIGHT)) player.angle += cameraSpeed;
    else if (IsKeyDown(KEY_LEFT)) player.angle -= cameraSpeed;
}
