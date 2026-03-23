#ifndef RESOURCES_H
#define RESOURCES_H

#include "raylib.h"
#include "settings.h"
#include "types.h"

void LoadResources();
Color GetAtlasPixel(const int index, const Vector2Int pixelPos);

#endif // RESOURCES_H