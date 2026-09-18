#ifndef ASTEROIDS_H
#define ASTEROIDS_H

#include "raylib.h"
#include "raymath.h"
#include "ship.h"

#define NVERTICES 12

extern int max_asteroids;
extern int init_asteroids;
extern int asteroids_count;
extern int total_score;

typedef struct {
    Vector2 position;
    float radius;
    // int n_vertices;
    Vector2 vertices[NVERTICES];
    int bitcodes[NVERTICES]; // indicates whether the ith vertex has crossed the top, bottom, left or right boundary
    Vector2 velocity;
    int state; // if state = 3, original size asteroid, if 2 medium sized, if 1 small sized, if 0, it is inactive or destroyed
} Asteroid_T;

Asteroid_T *Asteroid_new();
// void Asteroid_vertices_free(Asteroid_T* a);
void Asteroid_push(Asteroid_T asteroids[], Asteroid_T* a);
void Asteroid_track_count(Asteroid_T asteroids[]);
void Asteroid_copy(Asteroid_T* a1, Asteroid_T* a2);
void Asteroid_rand_init(Asteroid_T* a);
void Asteroid_move(Asteroid_T* a);
void Asteroid_draw(Asteroid_T* a, Color color);
void Asteroid_init_vertex_codes(Asteroid_T* a, int screen_w, int screen_h);
int Asteroid_is_partially_crossed(Asteroid_T* a);
int Asteroid_is_fully_crossed(Asteroid_T* a);
void Asteroid_screen_wraparound(Asteroid_T* a,  int screen_w, int screen_h);
void Asteroid_fragment_or_destruct(Asteroid_T asteroids[], Asteroid_T* a);
void Asteroid_strike_ship(Asteroid_T asteroids[], Ship_T *ship);
bool Asteroid_check_ship_collision(Asteroid_T asteroids[], Ship_T *ship);
#endif
