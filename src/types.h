#ifndef TYPES_H
#define TYPES_H

#include "raylib.h"

typedef struct {
    Vector2 position;
    float angle;
} Player;

typedef struct {
    float cosAngle;
    float sinAngle;
    float angle;
} Ray2D;

typedef struct {
    int x;
    int y;
} Cell;


typedef struct {
    bool polar;
    float distance;
    Cell cell;
} RaycastHit;

// typedef struct {
//     Vector2 start;
//     Vector2 end;
// } Wall;

// typedef struct {
//     Vector2 direction;
//     float angle;
//     float angleDiff;
// } PlayerRay;

#endif //TYPES_H
