#ifndef SETTINGS_H
#define SETTINGS_H

#include "raylib.h"
#include "math.h"

// Screen
static const int screenWidth = 1920;
static const int screenHeight = 1080;

// Render
static const int renderWidth = 800;
static const int renderHeight = 450;
static const int raysCount = renderWidth;
static const float fov = 90.0f;
static const int dof = 8;

// Movement
static const float collisionDistance = 0.5f;
static const float cameraSpeed = 0.5f;
static const float moveSpeed = 0.5f;

// Resources
static const char *resourcesPath = "resources";
static const char *textureAtlas = "atlas.png";
static const int textureSize = 16;

#endif // SETTINGS_H