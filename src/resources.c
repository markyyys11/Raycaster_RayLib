#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "resources.h"
#include "settings.h"

Image atlas;
int atlasWidth, atlasHeight;
int atlasRows, atlasCols;

void LoadResources() {
    char buffer[50];
    snprintf(buffer, sizeof(buffer), "%s/%s", resourcesPath, textureAtlas);
    atlas = LoadImage(buffer);
    atlasWidth = atlas.width;
    atlasHeight = atlas.height;
    atlasCols = atlasWidth / textureSize;
    atlasRows = atlasHeight / textureSize;
}

void GetTexture(const int index, const int width, const int height, Color *pixels) {
    if (atlas.data == NULL || pixels == NULL) {
        fprintf(stderr, "Ошибка: NULL указатель\n");
        return;
    }
    if (index < 0 || index >= atlasCols * atlasRows) {
        fprintf(stderr, "Ошибка: индекс %d вне диапазона\n", index);
        return;
    }
    
    int col = index % atlasCols;
    int row = index / atlasCols;
    
    // Начальные координаты в атласе
    int startX = col * textureSize;
    int startY = row * textureSize;
    
    Color *atlasPixels = (Color *)atlas.data;
    
    // Копирование построчно
    for (int y = 0; y < height; y++) {
        int srcIndex = (startY + y) * atlasWidth + startX;
        int dstIndex = y * width;
        
        memcpy(&pixels[dstIndex], &atlasPixels[srcIndex], width * sizeof(Color));
    }
}

void UnloadResources() {
    UnloadImage(atlas);
}