#include "raylib.h"
#include "raymath.h"
#include "asteroids.h"
#include "ship.h"
#include "bullets.h"
#include "utils.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#define DEBUG 0
extern int buffer;
// Asteroid_T *Asteroid_new() { return malloc(sizeof(Asteroid_T)); }



/* void Asteroid_vertices_free(Asteroid_T *a) { */
/*     free(a->vertices); */
/*     a->vertices = NULL; */
/*     free(a->bitcodes); */
/*     a->bitcodes = NULL; */
/* } */


void Asteroid_push(Asteroid_T asteroids[], Asteroid_T* a) {
    if (asteroids_count < max_asteroids) {
        asteroids[asteroids_count] = *a;
    } 
}


void Asteroid_track_count(Asteroid_T asteroids[]) {
    asteroids_count = 0;
    for (int i = 0; i < max_asteroids; i++) {
        if (asteroids[i].state > 0) asteroids_count++;
    }
}


void Asteroid_copy(Asteroid_T* a1, Asteroid_T* a2) {
    // copies the position, radius, vertices, bitcodes and velocity of a2 into a1
    a1->position = a2->position;
    a1->radius = a2->radius;
    // a1->n_vertices = a2->n_vertices;
    // a1->vertices = malloc(sizeof(Vector2) * NVERTICES);
    for (int i = 0; i < NVERTICES; i++) {
        a1->vertices[i] = a2->vertices[i];
    }
    // a1->bitcodes = malloc(sizeof(int) * a1->n_vertices);
    Asteroid_init_vertex_codes(a1, GetScreenWidth(), GetScreenHeight());
    for (int i = 0; i < NVERTICES; i++) {
        a1->bitcodes[i] = a2->bitcodes[i];
    }
    a1->velocity = a2->velocity;
    a1->state = a2->state;
}


void Asteroid_shape_generate(Asteroid_T* a){
    float x_min = a->position.x - a->radius;
    float x_max = a->position.x + a->radius;
    float x = x_min;
    a->vertices[0] = Vector2Create(x_min, a->position.y);
    a->vertices[NVERTICES - 1] = Vector2Create(x_max, a->position.y);
    int pos_y_n_max = (NVERTICES - 2) / 2;
    int neg_y_n_max = NVERTICES - 2;
    // TODO: Simplify the vertex selection logic
    for (int i = 1; i <= pos_y_n_max; i++) {
        x = get_random_float(x, x_max);
        float y = a->position.y + sqrtf(powf(a->radius, 2) - powf(x - a->position.x, 2)); 
        a->vertices[i] = Vector2Create(x, y);
    }
    x = x_min;
    for (int i = pos_y_n_max + 1; i <= neg_y_n_max; i++) {
        x = get_random_float(x, x_max);
        float y = a->position.y - sqrtf(powf(a->radius, 2) - powf(x - a->position.x, 2)); 
        a->vertices[i] = Vector2Create(x, y);
    }
    for (int i = 0; i < 4; i++) {
        int n = GetRandomValue(0, NVERTICES - 1);
        a->vertices[n] = Vector2ScaleRelative(a->vertices[n], get_random_float(0.5, 1), a->position);
    }
}



void Asteroid_rand_init(Asteroid_T* a) {
    int sc_w = GetScreenWidth();
    int sc_h = GetScreenHeight();
    int buffer = 200;
    Vector2 init_positions[4];
    Vector2 init_directions[4];
    init_positions[0] = Vector2Create(GetRandomValue(-buffer / 2, 0), GetRandomValue(0, sc_h));
    init_positions[1] = Vector2Create(GetRandomValue(0, sc_w), GetRandomValue(-buffer / 2, 0));
    init_positions[2] = Vector2Create(GetRandomValue(sc_w, sc_w + buffer / 2), GetRandomValue(0, sc_h));
    init_positions[3] = Vector2Create(GetRandomValue(0, sc_w), GetRandomValue(sc_h, sc_h + buffer / 2));
    a->position = init_positions[GetRandomValue(0, 3)];

    a->radius = get_random_float(50, 70);
    
    // a->vertices = malloc(sizeof(Vector2) * NVERTICES);
    // a->bitcodes = malloc(sizeof(int) * NVERTICES);

    
    float speed = get_random_float(60, 100);
    float rotation = get_random_float(20 * DEG2RAD, 60 * DEG2RAD);
    a->velocity = Vector2Rotate(Vector2Scale(Vector2Create(0, -1), speed), rotation);
    
    a->state = 3;

    Asteroid_shape_generate(a);
    
    Asteroid_init_vertex_codes(a, sc_w, sc_h);
}

void Asteroid_reposition(Asteroid_T* a, Vector2 displacement) {
    a->position = Vector2Add(a->position, displacement);
    for (int i = 0; i < NVERTICES; i++) {
        a->vertices[i] = Vector2Add(a->vertices[i], displacement);
    }
    // a->rotation = GetRandomValue;
    // for () {
    //    a->vertices[i] = Vector2RotateAround(a->vertices[i], a->rotation, a->position);
    // }
}

void Asteroid_rescale(Asteroid_T* a, float scale) {
    for (int i = 0; i < NVERTICES; i++) {
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
    int pos_y_n_max = (NVERTICES - 2) / 2;
    int neg_y_n_max = (NVERTICES % 2 == 0) ? NVERTICES / 2 : NVERTICES / 2 + 1;
    DrawLineV(a->vertices[0], a->vertices[1], color);
    DrawLineV(a->vertices[0], a->vertices[pos_y_n_max + 1], color);
    for (int i = 1; i < pos_y_n_max; i++) {
        DrawLineV(a->vertices[i], a->vertices[i+1], color);
    }
    for (int i = pos_y_n_max + 1; i <= NVERTICES - 2; i++) {
        DrawLineV(a->vertices[i], a->vertices[i+1], color);
    }
    DrawLineV(a->vertices[pos_y_n_max], a->vertices[NVERTICES - 1], color);
    DrawLineV(a->vertices[NVERTICES - 2], a->vertices[NVERTICES - 1], color);
    if (DEBUG) DrawCircleLinesV(a->position, a->radius, YELLOW);
    if (DEBUG) {
        for (int i = 0; i < NVERTICES; i++) {
            // sprintf(debug, "(%f, %f)", a->vertices[i].x, a->vertices[i].y);
            int code = vertex_code(a->vertices[i], GetScreenWidth(), GetScreenHeight());
            sprintf(debug, "Point %d, %s", i, ((code > 0)  && (code <= 15)) ? "crossed" : "uncrossed");
            DrawText(debug, a->vertices[i].x, a->vertices[i].y, 10, color);
        }
    }
}


void Asteroid_init_vertex_codes(Asteroid_T* a, int screen_w, int screen_h) {
    for (int i = 0; i < NVERTICES; i++) {
        a->bitcodes[i] = vertex_code(a->vertices[i], screen_w, screen_h);
    }
}


int Asteroid_is_partially_crossed(Asteroid_T* a) {
    for (int i = 0; i < NVERTICES; i++) {
        if ((a->bitcodes[i] > 0) && (a->bitcodes[i] < 15))
            return a->bitcodes[i];   
    }
    return 0;
}

int Asteroid_is_fully_crossed(Asteroid_T* a) {
    for (int i = 0; i < NVERTICES; i++) {
        if (a->bitcodes[i] == 0) return 0;
    }
    return 1;
    
}


void Asteroid_screen_wraparound(Asteroid_T *a,  int screen_w, int screen_h) {
    // Asteroid_copy(a2, a);
    switch (Asteroid_is_partially_crossed(a)) {
    case 10:
        Asteroid_reposition(a, Vector2Create(screen_w, screen_h));
        break;
    case 9:
        Asteroid_reposition(a, Vector2Create(screen_w, -screen_h));
        break;
    case 8: // partially crosses left boundary
        Asteroid_reposition(a, Vector2Create(screen_w, 0));
        break;
    case 6:
        Asteroid_reposition(a, Vector2Create(-screen_w, screen_h));
        break;
    case 5:
        Asteroid_reposition(a, Vector2Create(-screen_w, -screen_h));
        break;
    case 2: // partially crosses top boundary
        Asteroid_reposition(a, Vector2Create(0, screen_h));
        break;
    case 4: // partially crosses right boundary
        Asteroid_reposition(a, Vector2Create(-screen_w, 0));
        break;
    case 1: // partially crosses bottom boundary
        Asteroid_reposition(a, Vector2Create(0, -screen_h));
        break;
    }
}

void Asteroid_fragment_or_destruct(Asteroid_T asteroids[], Asteroid_T *a) {
    float rand_angle;
    float rand_velocity_factor;
    // Asteroid_rescale(a, 0.5);
    switch (a->state) {
    case 3:
        a->state--;
        asteroids_count++;
        Asteroid_T* medium_asteroid = &asteroids[asteroids_count];
        Asteroid_copy(medium_asteroid, a);
        Asteroid_shape_generate(a);
            
        rand_angle = get_random_float(20 * DEG2RAD, 45 * DEG2RAD);
        rand_velocity_factor = get_random_float(0.8, 1.6);
        medium_asteroid->velocity = Vector2Scale(Vector2Rotate(medium_asteroid->velocity, rand_angle), rand_velocity_factor);
        Asteroid_rescale(medium_asteroid, 0.5);
        // Asteroid_push(asteroids, &medium_asteroid);
        // Asteroid_push(asteroids_2, &medium_asteroid);
        
        rand_angle = get_random_float(20 * DEG2RAD, 45 * DEG2RAD);
        rand_velocity_factor = get_random_float(1.2, 1.6);
        a->velocity = Vector2Scale(Vector2Rotate(a->velocity, rand_angle), rand_velocity_factor);
        Asteroid_rescale(a, 0.5);

        // asteroids_count++;
        total_score += 100;
        break;
    case 2:
        a->state--;
        asteroids_count++;
        // asteroids = realloc(asteroids, asteroids_count + 1);
        // asteroids_2 = realloc(asteroids_2, asteroids_count + 1);
        Asteroid_T* small_asteroid = &asteroids[asteroids_count];
        Asteroid_copy(small_asteroid, a);
        Asteroid_shape_generate(a);
            
        rand_angle = get_random_float(20 * DEG2RAD, 45 * DEG2RAD);
        rand_velocity_factor = get_random_float(1.6, 2);
        small_asteroid->velocity = Vector2Scale(Vector2Rotate(small_asteroid->velocity, rand_angle), rand_velocity_factor);
        Asteroid_rescale(small_asteroid, 0.5);
        // Asteroid_push(asteroids_2, &small_asteroid);
        
        rand_angle = get_random_float(20 * DEG2RAD, 45 * DEG2RAD);
        rand_velocity_factor = get_random_float(0.8, 1.4);
        a->velocity = Vector2Scale(Vector2Rotate(a->velocity, rand_angle), rand_velocity_factor);
        Asteroid_rescale(a, 0.5);

        // asteroids_count++;
        total_score += 200;
        break;
    case 1:
        a->state--;
        asteroids_count--;
        total_score += 500;
        break;
    }
    // Asteroid_track_count(asteroids);
}



void Asteroid_strike_ship(Asteroid_T asteroids[], Ship_T *ship) {
    for (int i = 0; i < max_asteroids; i++) {
        float ship_radius = Vector2Distance(ship->top, ship->centroid) * 0.9;
        bool collision = CheckCollisionCircles(asteroids[i].position, asteroids[i].radius, ship->centroid, ship_radius);
        if ((asteroids[i].state && ship->intact) && collision) {
            ship->intact = 0;
            // asteroids[i].state = 0;
            lives--;
            Asteroid_fragment_or_destruct(asteroids, &asteroids[i]);
            break;
        }
    }
}

bool Asteroid_check_ship_collision(Asteroid_T asteroids[], Ship_T* ship) {
    for (int i = 0; i < max_asteroids; i++) {
        bool collision = CheckCollisionCircles(
            asteroids[i].position, asteroids[i].radius, ship->centroid, ship->radius);
        if (collision) return true;
    }
    return false;
}


