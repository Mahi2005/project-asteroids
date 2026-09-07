#ifndef SHIP_H
#define SHIP_H

#include "raylib.h"
#include "raymath.h"

typedef struct {
  Vector2 centroid;
  Vector2 top;
  Vector2 left;
  Vector2 right;
  Vector2 velocity;
  float rotation;
  float rotation_speed;
  Vector2 acceleration;
  float max_speed;
  bool intact;
} Ship_T;

void reposition_ship(Ship_T*, Vector2, float);
Vector2 farthest_vertex_from_bottom(Ship_T *, int);
Vector2 farthest_vertex_from_top(Ship_T *, int);
Vector2 farthest_vertex_from_right(Ship_T *, int);
Vector2 farthest_vertex_from_left(Ship_T *, int);
int crossed_border_partial(Ship_T *, int, int);
void init_ship(Ship_T *ship, int shiplength);
void move_ship(Ship_T *);
void ship_screen_wraparound(Ship_T *, Ship_T *, int, int);
void destruct_ship(Ship_T *);
void DrawDestroyedShip(Ship_T *ship);
#endif
