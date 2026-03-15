#ifndef SETTINGS_H
#define SETTINGS_H

static const int screenWidth = 1400;
static const int screenHeight = 600;
static const int textureWidth = 400;
static const int textureHeight = 300;

static const int raysCount = 200;
static const float fov = 50.0f;
static const int dof = 10;

static const float cameraSpeed = 0.5f;
static const float moveSpeed = 0.5f;

static const int mapWidth = 10;
static const int mapHeight = 10;
static const int tileSize = 60;

static const float wallHeightMultiplier = 0.07f; // Магическое число для высоты стен

#endif // SETTINGS_H