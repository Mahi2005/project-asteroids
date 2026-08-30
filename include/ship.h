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
} Ship;

void reposition_ship(Ship*, Vector2, float);
Vector2 farthest_vertex_from_bottom(Ship *, int);
Vector2 farthest_vertex_from_top(Ship *, int);
Vector2 farthest_vertex_from_right(Ship *, int);
Vector2 farthest_vertex_from_left(Ship *, int);
int crossed_border_partial(Ship *, int, int);
void init_ship(Ship *, int);
void move_ship(Ship *);
void ship_screen_wraparound(Ship *, Ship *, int, int);
void destruct_ship(Ship *);
void DrawDestroyedShip(Ship *ship);
#endif
