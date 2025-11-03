#include "raylib.h"
#ifndef TRANSFORM_H
#define TRANSFORM_H

// position, rotation, scale
struct Transform2D{
    Vector2 position = {0,0};
    float   rotation = 0.0f,
            scale    = 1.0f;
};
#endif