#ifndef RENDER2D_H
#define RENDER2D_H

#include "types.h"
#include "settings.h"

void Draw2D(const Player player, const Ray2D rays[raysCount], const RaycastHit hits[raysCount]);

#endif //RENDER2D_H