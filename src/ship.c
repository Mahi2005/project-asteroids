#include <stdlib.h>
#include "ship.h"
#include "utils.h"
#include "asteroids.h"

void Ship_reposition(Ship_T *ship, Vector2 displacement, float angle) {
  // orients the ship's top, left and right vertices after updating its centroid
  // i.e position by translation or rotation
  // direction of ship is determined by direction of its top vertex w.r.t its
  // centroid
  ship->centroid = Vector2Add(ship->centroid, displacement);
  ship->top = Vector2Add(ship->top, displacement);
  ship->top = Vector2RotateAround(ship->top, angle, ship->centroid);
  ship->right = Vector2RotateAround(ship->top, 150 * DEG2RAD, ship->centroid);
  ship->left = Vector2RotateAround(ship->top, -150 * DEG2RAD, ship->centroid);
}



void Ship_init_vertex_codes(Ship_T* ship, int screen_w, int screen_h) {
    ship->bitcodes[0] = vertex_code(ship->top, screen_w, screen_h);
    ship->bitcodes[1] = vertex_code(ship->right, screen_w, screen_h);
    ship->bitcodes[2] = vertex_code(ship->left, screen_w, screen_h);
}

int Ship_is_partially_crossed(Ship_T* ship) {
    for (int i = 0; i < 3; i++) {
        if (ship->bitcodes[i] > 0) return ship->bitcodes[i];   
    }
    return 0;
}

int Ship_is_fully_crossed(Ship_T* ship) {
    for (int i = 0; i < 3; i++) {
        if (ship->bitcodes[i] == 0) return 0;
    }
    return 1;   
}

void Ship_init(Ship_T *ship) {
    float ship_length = 20;
    float rotation = (1 / 10.0) * (PI / 6);
    float init_speed = 10;
    float acceleration = 500;
    float max_speed = 800;
    ship->centroid = Vector2Create(GetScreenWidth() / 2.0, GetScreenHeight() / 2.0);
    ship->top = Vector2Add(ship->centroid, Vector2Create(0, -ship_length));
    Ship_reposition(ship, Vector2Zero(), rotation);
    ship->radius = ship_length;
    ship->velocity = Vector2Create(0, -init_speed);
    ship->acceleration = Vector2Create(0, -acceleration);
    ship->rotation = rotation;
    ship->intact = true;
    ship->max_speed = max_speed;
}


void Ship_copy(Ship_T* ship1, Ship_T* ship2) {
    // copies the position, radius, vertices, bitcodes and velocity of a2 into a1
    ship1->centroid = ship2->centroid;
    ship1->top = ship2->top;
    ship1->right = ship2->right;
    ship1->left = ship2->left;
    for (int i = 0; i < 3; i++) {
        ship1->bitcodes[i] = ship2->bitcodes[i];
    }
    ship1->velocity = ship2->velocity;
    ship1->rotation = ship2->rotation;
    ship1->acceleration = ship2->acceleration;
    ship1->max_speed = ship2->max_speed;
    ship1->intact = ship2->intact;
}

void Ship_move(Ship_T *ship) {
    float dt = GetFrameTime();
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
        ship->acceleration = Vector2Rotate(ship->acceleration, -ship->rotation);
        Ship_reposition(ship, Vector2Zero(), -ship->rotation);
    }
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
        ship->acceleration = Vector2Rotate(ship->acceleration, ship->rotation);
        Ship_reposition(ship, Vector2Zero(), ship->rotation);
    }
    
    if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) {
        if (Vector2Length(ship->velocity) < ship->max_speed) {
            Vector2 delta_v = Vector2Scale(ship->acceleration, dt);
            ship->velocity = Vector2Add(ship->velocity, delta_v);
        }
        Vector2 displacement = Vector2Scale(ship->velocity, dt);
        Ship_reposition(ship, displacement, 0);
    } else {
        float damping = expf(-1.2 * dt);
        ship->velocity = Vector2Scale(ship->velocity, damping);
        Vector2 displacement = Vector2Scale(ship->velocity, dt);
        Ship_reposition(ship, displacement, 0);
    }
}

void Ship_screen_wraparound(Ship_T *ship, int screen_w, int screen_h) {
    // Ship_copy(ship_cpy, ship);
    switch (Ship_is_partially_crossed(ship)) {
    case 10: // partially crosses top-left corner
        Ship_reposition(ship, Vector2Create(screen_w, screen_h), 0);
        break;
    case 9:
        Ship_reposition(ship, Vector2Create(screen_w, -screen_h), 0);
        break;
    case 8: // partially crosses left boundary
        Ship_reposition(ship, Vector2Create(screen_w, 0), 0);
        break;
    case 6:
        Ship_reposition(ship, Vector2Create(-screen_w, screen_h), 0);
        break;
    case 5:
        Ship_reposition(ship, Vector2Create(-screen_w, -screen_h), 0);
        break;
    case 2: // partially crosses top boundary
        Ship_reposition(ship, Vector2Create(0, screen_h), 0);
        break;
    case 4: // partially crosses right boundary
        Ship_reposition(ship, Vector2Create(-screen_w, 0), 0);
        break;
    case 1: // partially crosses bottom boundary
        Ship_reposition(ship, Vector2Create(0, -screen_h), 0);
        break;
    }
}

void Ship_destroy(Ship_T *ship) {
  // TODO: Implement ship destruction effects here
  // Main idea is to detach the lines of the ship and show them being rotated or
  // displaced in random directions.
  ship->intact = false;
}


