#ifndef RESOURCES_H
#define RESOURCES_H

#include "raylib.h"
#include "settings.h"

void LoadResources();
void GetTexture(const int index, const int width, const int height, Color *pixels);
void UnloadResources();

#endif // RESOURCES_H