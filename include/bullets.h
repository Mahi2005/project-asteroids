#ifndef BULLETS_H
#define BULLETS_H

#include "raylib.h"
#include "raymath.h"
#include "utils.h"
#include "ship.h"

#define MAX_BULLETS 20
#define BULLET_SPEED 400

typedef struct {
  Vector2 position;
  Vector2 velocity;
  float radius;
  int active;
  float lifetime;
} Bullets;

void shoot_bullets(Bullets *bullet, Ship *ship);
void bullets_screen_wraparound(Bullets *bullet, int screenwidth, int screenheight);
void bullet_destroy_ship(Bullets *bullet, Ship *ship);


#endif