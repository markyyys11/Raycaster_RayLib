#include "utils.h"
#include "raylib.h"

Color ColorMultiply(Color a, Color b) {
    return (Color) {.r = (a.r * b.r) / 255, .g = (a.g * b.g) / 255, .b = (a.b * b.b) / 255, 255};
}
