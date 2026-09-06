#include "raylib.h"
#include "raymath.h"
#include "asteroids.h"
#include "utils.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

extern int ASTEROIDS_COUNT = 0; 
static int DEBUG = 0;

Asteroid_T* Asteroid_new() {
    return malloc(sizeof(Asteroid_T));
}

void Asteroid_init(Vector2 pos, float radius, Vector2 *vertices, int n, Vector2 velocity) {
    
}

void Asteroid_rand_init(Asteroid_T* a) {
    
    a->position = Vector2Create(GetRandomValue(0, GetScreenWidth()), GetRandomValue(0, GetScreenHeight()));
    a->radius = GetRandomValue(1000, 2000) / 10.0;
    a->n_vertices = 12;
    a->vertices = malloc(sizeof(Vector2) * a->n_vertices);

    
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
    float speed = GetRandomValue(100, 999) / 10.0;
    //float speed = 0;
    float rotation = GetRandomValue(0, (2*PI)*100) / 100.0;
    // float rotation = 0;
    a->velocity = Vector2Rotate(Vector2Scale(Vector2Create(0, -1), speed), rotation);
}

void Asteroid_reposition(Asteroid_T* a, Vector2 displacement) {
    a->position = Vector2Add(a->position, displacement);
    for (int i = 0; i < a->n_vertices; i++) {
        a->vertices[i] = Vector2Add(a->vertices[i], displacement);
    }
}

void Asteroid_move(Asteroid_T* a) {
    float dt = GetFrameTime();
    Vector2 delta_r = Vector2Scale(a->velocity, dt);
    Asteroid_reposition(a, delta_r);
}



void Asteroid_draw(Asteroid_T* a) {
    char debug[1024];
    int pos_y_n_max = (a->n_vertices - 2) / 2;
    int neg_y_n_max = (a->n_vertices % 2 == 0) ? a->n_vertices / 2 : a->n_vertices / 2 + 1;
    DrawLineV(a->vertices[0], a->vertices[1], WHITE);
    DrawLineV(a->vertices[0], a->vertices[pos_y_n_max + 1], WHITE);
    for (int i = 1; i < pos_y_n_max; i++) {
        DrawLineV(a->vertices[i], a->vertices[i+1], WHITE);
    }
    for (int i = pos_y_n_max + 1; i <= a->n_vertices - 2; i++) {
        DrawLineV(a->vertices[i], a->vertices[i+1], WHITE);
    }
    DrawLineV(a->vertices[pos_y_n_max], a->vertices[a->n_vertices - 1], WHITE);
    DrawLineV(a->vertices[a->n_vertices - 2], a->vertices[a->n_vertices - 1], WHITE);
    // DrawCircleLinesV(a->position, a->radius, YELLOW);
    if (DEBUG) {
        for (int i = 0; i < a->n_vertices; i++) {
            // sprintf(debug, "(%f, %f)", a->vertices[i].x, a->vertices[i].y);
            sprintf(debug, "Point %d", i);
            DrawText(debug, a->vertices[i].x, a->vertices[i].y, 10, WHITE);
        }
    }
}


void Asteroid_screen_wraparound(Asteroid_T asteroids[]) {
    int screenwidth = GetScreenWidth();
    int screenheight = GetScreenHeight();
    for (int i = 0; i < MAX_ASTEROIDS; i++) {
        if (asteroids[i].position.x > screenwidth)
            Asteroid_reposition(&asteroids[i], Vector2Create(-screenwidth, 0));
        if (asteroids[i].position.x < 0)
            Asteroid_reposition(&asteroids[i], Vector2Create(screenwidth, 0));
        if (asteroids[i].position.y > screenheight)
            Asteroid_reposition(&asteroids[i], Vector2Create(0, -screenheight));
        if (asteroids[i].position.y < 0)
            Asteroid_reposition(&asteroids[i], Vector2Create(0, screenheight));
    }
}

