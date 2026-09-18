#include "bullets.h"
#include "ship.h"
#include "asteroids.h"
#include <stdlib.h>



void Bullet_screen_wraparound(Bullet_T bullets[], int screenwidth,
                               int screenheight) {
    for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullets[i].active) {
            if (bullets[i].position.x > screenwidth)
                bullets[i].position = Vector2Add(bullets[i].position, Vector2Create(-screenwidth, 0));
            if (bullets[i].position.x < 0)
                bullets[i].position = Vector2Add(bullets[i].position, Vector2Create(screenwidth, 0));
            if (bullets[i].position.y > screenheight)
                bullets[i].position = Vector2Add(bullets[i].position, Vector2Create(0, -screenheight));
            if (bullets[i].position.y < 0)
                bullets[i].position = Vector2Add(bullets[i].position, Vector2Create(0, screenheight));
        }
    }
}

void Bullet_strike_ship(Bullet_T *bullet, Ship_T *ship) {
    if (bullet->active) {
        bool bullet_collision = CheckCollisionPointTriangle(bullet->position, ship->top, ship->right, ship->left);
        if (bullet_collision) {
            ship->intact = 0;
            bullet->active = 0;
        }
    }
}


void Bullet_strike_asteroids(Bullet_T bullets[], Asteroid_T asteroids[]) {
    float error_factor = 0.1;
    for (int i = 0; i < MAX_BULLETS; i++) {
        for (int j = 0; j < max_asteroids; j++) {
            if ((bullets[i].active && asteroids[j].state) && CheckCollisionPointCircle(bullets[i].position, asteroids[j].position, asteroids[j].radius * (1 - error_factor))) {
                // asteroids[j].state--;
                // if (!asteroids[j].state) asteroids_count--;
                bullets[i].active = 0;
                Asteroid_fragment_or_destruct(asteroids, &asteroids[j]);
            }
        }
    }
}

void Bullet_shoot(Bullet_T bullets[], Ship_T *ship) {
  Vector2 ship_direction = Vector2Normalize(ship->acceleration);
  float dt = GetFrameTime();
  for (int i = 0; i < MAX_BULLETS; i++) {
      if (IsKeyPressed(KEY_SPACE)) {
          if (!bullets[i].active) {
              bullets[i].position =
              Vector2Add(ship->centroid, Vector2Scale(ship_direction, ship->radius));
              bullets[i].active = 1;
              bullets[i].velocity = Vector2Add(Vector2Scale(ship_direction, BULLET_SPEED), ship->velocity);
              bullets[i].lifetime = 0.0f;
              break;
          }
      }
      bullets[i].position = Vector2Add(bullets[i].position, Vector2Scale(bullets[i].velocity, dt));
      bullets[i].lifetime += dt;
      if (bullets[i].lifetime > 2.0) {
          bullets[i].active = 0;
      }
  }
}


