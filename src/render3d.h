#ifndef RENDER3D_H
#define RENDER3D_H

#include "types.h"
#include "settings.h"

void LoadTextureBuffer();
void Draw3D(const int screenWidth, const int screenHeight, const Player player, const Ray2D rays[], const RaycastHit hits[], Color *pixels);
void UnloadTextureBuffer();

#endif //RENDER3D_H