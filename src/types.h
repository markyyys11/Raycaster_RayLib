#ifndef TYPES_H
#define TYPES_H

#include "raylib.h"

typedef struct {
    Vector2 position;
    float cameraDegree;
} Player;

typedef struct {
    Vector2 start;
    Vector2 end;
} Wall;

typedef struct {
    Vector2 direction;
    float angle;
    float angleDiff;
} PlayerRay;

#endif //TYPES_H
