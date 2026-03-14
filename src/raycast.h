#ifndef RAYS_H
#define RAYS_H

#include "types.h"
#include "settings.h"

void Raycast(const Player player, Ray2D rays[raysCount], RaycastHit hits[raysCount]);

#endif //RAYS_H