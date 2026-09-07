#include "raylib.h"
#include "raymath.h"
#include "asteroids.h"
#include "bullets.h"
#include "utils.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <string.h>

#define DEBUG 0

extern int lives;

Asteroid_T* Asteroid_new() {
    return malloc(sizeof(Asteroid_T));
}

void Asteroid_init_to_zero(Asteroid_T* a){
    a->position = Vector2Zero();
    a->radius = 0;
    a->n_vertices = 0;
    a->vertices = NULL;
    a->velocity = Vector2Zero();
    a->state = 0;
}

void Asteroid_copy(Asteroid_T* a1, Asteroid_T* a2) {
    // copies the position, radius, vertices, bitcodes and velocity of a2 into a1
    a1->position = a2->position;
    a1->radius = a2->radius;
    a1->n_vertices = a2->n_vertices;
    a1->vertices = malloc(sizeof(Vector2) * a1->n_vertices);
    for (int i = 0; i < a1->n_vertices; i++) {
        a1->vertices[i] = a2->vertices[i];
    }
    Asteroid_init_vertex_codes(a1, GetScreenWidth(), GetScreenHeight());
    for (int i = 0; i < a1->n_vertices; i++) {
        a1->bitcodes[i] = a2->bitcodes[i];
    }
    a1->velocity = a2->velocity;
    a1->state = a2->state;
}


void Asteroid_rand_init(Asteroid_T* a) {
    Vector2 init_positions[2];
    int sc_w = GetScreenWidth();
    int sc_h = GetScreenHeight();
    init_positions[0] = Vector2Create(GetRandomValue(0, sc_w / 2 - 150), GetRandomValue(0, sc_h / 2 - 150));
    init_positions[1] = Vector2Create(GetRandomValue(sc_w / 2 + 150, sc_w), GetRandomValue(sc_h / 2 + 150, sc_h));
    a->position = init_positions[GetRandomValue(0, 1)];                                 
    a->radius = GetRandomValue(500, 1000) / 10.0;
    a->n_vertices = 12;
    if (a->vertices == NULL) a->vertices = malloc(sizeof(Vector2) * a->n_vertices);
    
    float x_min = a->position.x - a->radius;
    float x_max = a->position.x + a->radius;
    float x = x_min;
    a->vertices[0] = Vector2Create(x_min, a->position.y);
    a->vertices[a->n_vertices - 1] = Vector2Create(x_max, a->position.y);
    int pos_y_n_max = (a->n_vertices - 2) / 2;
    int neg_y_n_max = a->n_vertices - 2;
    for (int i = 1; i <= pos_y_n_max; i++) {
        x = get_random_float(x, x_max);
        float y = a->position.y + sqrtf(powf(a->radius, 2) - powf(x - a->position.x, 2)); 
        a->vertices[i] = Vector2Create(x, y);
    }
    x = a->position.x - a->radius;
    for (int i = pos_y_n_max + 1; i <= neg_y_n_max; i++) {
        x = get_random_float(x, x_max);
        float y = a->position.y - sqrtf(powf(a->radius, 2) - powf(x - a->position.x, 2)); 
        a->vertices[i] = Vector2Create(x, y);
    }
    for (int i = 0; i < 4; i++) {
        int n = GetRandomValue(0, a->n_vertices);
        a->vertices[n] = Vector2ScaleRelative(a->vertices[n], GetRandomValue(50, 100) / 100.0, a->position);
    }
    float speed = GetRandomValue(300, 600) / 10.0;
    //float speed = 0;
    float rotation = GetRandomValue(0, (2*PI)*100) / 100.0;
    // float rotation = 0;
    a->velocity = Vector2Rotate(Vector2Scale(Vector2Create(0, -1), speed), rotation);
    a->state = 3;
}

void Asteroid_reposition(Asteroid_T* a, Vector2 displacement) {
    a->position = Vector2Add(a->position, displacement);
    for (int i = 0; i < a->n_vertices; i++) {
        a->vertices[i] = Vector2Add(a->vertices[i], displacement);
    }
}

void Asteroid_rescale(Asteroid_T* a, float scale) {
    for (int i = 0; i < a->n_vertices; i++) {
        a->vertices[i] = Vector2ScaleRelative(a->vertices[i], scale, a->position);
    }
    a->radius *= scale;
}



void Asteroid_move(Asteroid_T* a) {
    float dt = GetFrameTime();
    Vector2 delta_r = Vector2Scale(a->velocity, dt);
    Asteroid_reposition(a, delta_r);
}



void Asteroid_draw(Asteroid_T* a, Color color) {
    char debug[1024];
    int pos_y_n_max = (a->n_vertices - 2) / 2;
    int neg_y_n_max = (a->n_vertices % 2 == 0) ? a->n_vertices / 2 : a->n_vertices / 2 + 1;
    DrawLineV(a->vertices[0], a->vertices[1], color);
    DrawLineV(a->vertices[0], a->vertices[pos_y_n_max + 1], color);
    for (int i = 1; i < pos_y_n_max; i++) {
        DrawLineV(a->vertices[i], a->vertices[i+1], color);
    }
    for (int i = pos_y_n_max + 1; i <= a->n_vertices - 2; i++) {
        DrawLineV(a->vertices[i], a->vertices[i+1], color);
    }
    DrawLineV(a->vertices[pos_y_n_max], a->vertices[a->n_vertices - 1], color);
    DrawLineV(a->vertices[a->n_vertices - 2], a->vertices[a->n_vertices - 1], color);
    // DrawCircleLinesV(a->position, a->radius, YELLOW);
    if (DEBUG) {
        for (int i = 0; i < a->n_vertices; i++) {
            // sprintf(debug, "(%f, %f)", a->vertices[i].x, a->vertices[i].y);
            int code = vertex_code(a->vertices[i], GetScreenWidth(), GetScreenHeight());
            sprintf(debug, "Point %d, %s", i, ((code > 0)  && (code <= 15)) ? "crossed" : "uncrossed");
            DrawText(debug, a->vertices[i].x, a->vertices[i].y, 10, color);
        }
    }
}


void Asteroid_init_vertex_codes(Asteroid_T* a, int screen_w, int screen_h) {
    a->bitcodes = malloc(sizeof(int) * a->n_vertices);
    for (int i = 0; i < a->n_vertices; i++) {
        a->bitcodes[i] = vertex_code(a->vertices[i], screen_w, screen_h);
    }
}


int Asteroid_is_partially_crossed(Asteroid_T* a) {
    for (int i = 0; i < a->n_vertices; i++) {
        if ((a->bitcodes[i] > 0) && (a->bitcodes[i] < 15))
            return a->bitcodes[i];   
    }
    return 0;
}

int Asteroid_is_fully_crossed(Asteroid_T* a) {
    for (int i = 0; i < a->n_vertices; i++) {
        if (a->bitcodes[i] == 0) return 0;
    }
    return 1;
    
}


void Asteroid_screen_wraparound(Asteroid_T *a, Asteroid_T *a2,  int screen_w, int screen_h) {
    Asteroid_copy(a2, a);
    switch (Asteroid_is_partially_crossed(a)) {
    case 8: // partially crosses left boundary
        Asteroid_reposition(a2, Vector2Create(screen_w, 0));
        break;
    case 2: // partially crosses top boundary
        Asteroid_reposition(a2, Vector2Create(0, screen_h));
        break;
    case 4: // partially crosses right boundary
        Asteroid_reposition(a2, Vector2Create(-screen_w, 0));
        break;
    case 1: // partially crosses bottom boundary
        Asteroid_reposition(a2, Vector2Create(0, -screen_h));
        break;
    }
}

void Asteroid_fragment_or_destruct(Asteroid_T *a) {
    if (a->state > 0) {
        a->state--;
        int score = (a->state == 2) ? 100 : ((a->state == 1) ? 200 : 500);
        total_score += score;
        Asteroid_rescale(a, 0.5);
        if (a->state == 0) {
            asteroids_count--;
        }
    }
}


void Asteroid_strike_ship(Asteroid_T asteroids[], Ship_T *ship) {
    for (int i = 0; i < MAX_ASTEROIDS; i++) {
        float ship_radius = Vector2Distance(ship->top, ship->centroid) * 0.9;
        bool collision = CheckCollisionCircles(asteroids[i].position, asteroids[i].radius, ship->centroid, ship_radius);
        if (asteroids[i].state && collision) {
            ship->intact = 0;
            // asteroids[i].state = 0;
            lives--;
            Asteroid_init_to_zero(&asteroids[i]);
            break;
        }
    }
}




