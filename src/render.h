#ifndef RENDERBYRAYS_H
#define RENDERBYRAYS_H

#include "types.h"

extern Wall *walls;
extern int wallsCount;

void InitWalls(int count);
void FreeWalls(void);
void DrawWalls(Player player);

#endif //RENDERBYRAYS_H
