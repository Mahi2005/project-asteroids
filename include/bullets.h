#ifndef BULLETS_H
#define BULLETS_H

#include "raylib.h"
#include "raymath.h"
#include "utils.h"
#include "ship.h"
#include "asteroids.h"

#define MAX_BULLETS 20
#define BULLET_SPEED 400

typedef struct {
  Vector2 position;
  Vector2 velocity;
  float radius;
  int active;
  float lifetime;
} Bullet_T;

void Bullet_shoot(Bullet_T *bullet, Ship_T *ship);
void Bullet_screen_wraparound(Bullet_T *bullet, int screenwidth, int screenheight);
void Bullet_strike_ship(Bullet_T *bullet, Ship_T *ship);
void Bullet_strike_asteroids(Bullet_T bullets[], Asteroid_T asteroids[], Asteroid_T asteroids_2[]);

#endif
