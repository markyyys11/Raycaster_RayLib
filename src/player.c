#include <math.h>
#include "raylib.h"
#include "types.h"
#include "player.h"
#include "settings.h"

Player player = { .position = { 400.0f, 300.0f }, .cameraDegree = 0.0f };

void MovePlayer(void) {
    if (IsKeyDown(KEY_W)) {
        player.position.x += moveSpeed * cosf(player.cameraDegree * DEG2RAD);
        player.position.y += moveSpeed * sinf(player.cameraDegree * DEG2RAD);
    }
    else if (IsKeyDown(KEY_S)) {
        player.position.x -= moveSpeed * cosf(player.cameraDegree * DEG2RAD);
        player.position.y -= moveSpeed * sinf(player.cameraDegree * DEG2RAD);
    }

    if (IsKeyDown(KEY_D)) {
        player.position.x += moveSpeed * cosf((player.cameraDegree + 90) * DEG2RAD);
        player.position.y += moveSpeed * sinf((player.cameraDegree + 90) * DEG2RAD);
    }
    else if (IsKeyDown(KEY_A)) {
        player.position.x -= moveSpeed * cosf((player.cameraDegree + 90) * DEG2RAD);
        player.position.y -= moveSpeed * sinf((player.cameraDegree + 90) * DEG2RAD);
    }

    if (IsKeyDown(KEY_RIGHT)) player.cameraDegree += cameraSpeed;
    else if (IsKeyDown(KEY_LEFT)) player.cameraDegree -= cameraSpeed;
}
