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