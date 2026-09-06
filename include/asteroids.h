#ifndef ASTEROIDS_H
#define ASTEROIDS_H

#include "raylib.h"
#include "raymath.h"

#define MAX_ASTEROIDS 6

typedef struct {
    Vector2 position;
    float radius;
    int n_vertices;
    Vector2 *vertices;
    Vector2 speed;
    Vector2 velocity;
} Asteroid_T;

Asteroid_T* Asteroid_new();
void Asteroid_rand_init(Asteroid_T* a);
void Asteroid_move(Asteroid_T* a);
void Asteroid_draw(Asteroid_T* a);
void Asteroid_screen_wraparound(Asteroid_T* a);

#endif
