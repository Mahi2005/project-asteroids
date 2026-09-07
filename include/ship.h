#ifndef SHIP_H
#define SHIP_H

#include "raylib.h"
#include "raymath.h"

typedef struct {
    Vector2 centroid;
    Vector2 top;
    Vector2 left;
    Vector2 right;
    int *bitcodes;
    Vector2 velocity;
    float rotation;
    float rotation_speed;
    Vector2 acceleration;
    float max_speed;
    bool intact;
} Ship_T;

void Ship_reposition(Ship_T*, Vector2, float);
// Vector2 farthest_vertex_from_bottom(Ship_T *, int);
// Vector2 farthest_vertex_from_top(Ship_T *, int);
// Vector2 farthest_vertex_from_right(Ship_T *, int);
// Vector2 farthest_vertex_from_left(Ship_T *, int);
void Ship_init_vertex_codes(Ship_T* ship, int screen_w, int screen_h);
int Ship_is_partially_crossed(Ship_T* ship);
int Ship_is_fully_crossed(Ship_T* ship);
void Ship_init(Ship_T* ship);
void Ship_move(Ship_T* ship);
void Ship_screen_wraparound(Ship_T* ship, Ship_T* ship_cpy, int screen_w, int screen_h);
void Ship_destroy(Ship_T *ship);
#endif
