#ifndef SETTINGS_H
#define SETTINGS_H

// Screen
static const int screenWidth = 1400;
static const int screenHeight = 600;
static const int textureWidth = 400;
static const int textureHeight = 300;

// Render
static const int raysCount = 200;
static const float fov = 50.0f;
static const int dof = 10;
static const float wallHeightMultiplier = 0.07f; // "Магическое число" для высоты стен

// Movement
static const float collisionDistance = 0.5f;
static const float cameraSpeed = 0.5f;
static const float moveSpeed = 0.5f;

// Resources
static const char *resourcesPath = "resources";
static const char *textureAtlas = "atlas.png";
static const int textureSize = 16;

#endif // SETTINGS_H