#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "resources.h"
#include "settings.h"
#include "types.h"

Color *atlasData;
// Image atlas;
int atlasWidth, atlasHeight;
int atlasRows, atlasCols;

void LoadResources() {
    char buffer[50];
    snprintf(buffer, sizeof(buffer), "%s/%s", resourcesPath, textureAtlas);
    Image atlas = LoadImage(buffer);
    atlasWidth = atlas.width;
    atlasHeight = atlas.height;
    atlasCols = atlasWidth / textureSize;
    atlasRows = atlasHeight / textureSize;
    atlasData = calloc(atlasWidth * atlasHeight, sizeof(Color));
    memcpy(atlasData, (Color *)atlas.data, sizeof(Color) * atlasWidth * atlasHeight) ;
    UnloadImage(atlas);
}

Color GetAtlasPixel(const int index, const Vector2Int pixelPos) {
    if (index < 0 || index >= atlasCols * atlasRows) {
        fprintf(stderr, "Ошибка: индекс %d вне диапазона\n", index);
        return RED;
    }
    
    int col = index % atlasCols;
    int row = index / atlasCols;
    
    // Начальные координаты в атласе
    int startX = col * textureSize;
    int startY = row * textureSize;

    return atlasData[(startY + pixelPos.y) * atlasWidth + (startX + pixelPos.x)];
}