#ifndef TYPES_H
#define TYPES_H

#include "raylib.h"

typedef struct {
    Vector3 position;
    float angle;
} Player;

typedef struct {
    float cosAngle;
    float sinAngle;
    float tanAngle;
    float angle;
} Ray2D;

typedef struct {
    int x;
    int y;
} Vector2Int;

typedef struct {
    bool polar;
    float distance;
    Vector2 position;
} RaycastHit;

typedef struct {
    float top;
    float height;
} WallRow;

#endif //TYPES_H
