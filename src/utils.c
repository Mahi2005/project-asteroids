#include "utils.h"

Vector2 Vector2Create(float x, float y) {
  Vector2 v = {.x = x, .y = y};
  return v;
}

Vector2 Vector2RotateAround(Vector2 point, float angle, Vector2 center) {
  return Vector2Add(Vector2Rotate(Vector2Subtract(point, center), angle),
                    center);
}

Vector2 Vector2ScaleRelative(Vector2 point, float scale, Vector2 center) {
  Vector2 relative = Vector2Subtract(point, center);
  relative = Vector2Scale(relative, scale);
  return Vector2Add(relative, center);
}

float get_random_float(float min, float max) {
    return GetRandomValue(min * 100, max * 100) / 100.0;
}

int vertex_code(Vector2 v, int screen_w, int screen_h) {
    int b3, b2, b1, b0;
    b3 = (v.x < 0) << 3; // whether crosses left boundary
    b2 = (v.x > screen_w) << 2; // whether crosses right boundary
    b1 = (v.y < 0) << 1; // whether crosses top boundary
    b0 = (v.y > screen_h); // whether crosses bottom boundary
    return b3 | b2 | b1 | b0;
}
