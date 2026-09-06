#ifndef UTILS_H
#define UTILS_H

#include "raylib.h"
#include "raymath.h"

Vector2 Vector2Create(float x, float y);
Vector2 Vector2RotateAround(Vector2 point, float angle, Vector2 center);
Vector2 Vector2ScaleRelative(Vector2 point, float scale, Vector2 center);   
float get_random_float(float min, float max);
int vertex_code(Vector2 v, int screen_w, int screen_h);
#endif
