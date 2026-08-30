#include "bullets.h"

void bullets_screen_wraparound(Bullets *bullet, int screenwidth,
                               int screenheight) {
  for (int i = 0; i < MAX_BULLETS; i++) {
    if (bullet[i].active) {
      if (bullet[i].position.x > screenwidth)
        bullet[i].position =
            Vector2Add(bullet[i].position, Vector2Create(-screenwidth, 0));
      if (bullet[i].position.x < 0)
        bullet[i].position =
            Vector2Add(bullet[i].position, Vector2Create(screenwidth, 0));
      if (bullet[i].position.y > screenheight)
        bullet[i].position =
            Vector2Add(bullet[i].position, Vector2Create(0, -screenheight));
      if (bullet[i].position.y < 0)
        bullet[i].position =
            Vector2Add(bullet[i].position, Vector2Create(0, screenheight));
    }
  }
}

void bullet_destroy_ship(Bullets *bullet, Ship *ship) {
  for (int i = 0; i < MAX_BULLETS; i++) {
    if (bullet[i].active) {
      bool bullet_collision = CheckCollisionPointTriangle(
          bullet[i].position, ship->top, ship->right, ship->left);
      if (bullet_collision) {
        destruct_ship(ship);
        bullet[i].active = 0;
      }
    }
  }
}

void shoot_bullets(Bullets *bullet, Ship *ship) {
  Vector2 ship_direction = Vector2Normalize(ship->acceleration);
  float dt = GetFrameTime();
  for (int i = 0; i < MAX_BULLETS; i++) {
      if (IsKeyPressed(KEY_SPACE)) {
          if (!bullet[i].active) {
              float shipradius = 50;
              bullet[i].position =
                  Vector2Add(ship->centroid, Vector2Scale(ship_direction, shipradius));
              bullet[i].active = 1;
              bullet[i].velocity = Vector2Add(Vector2Scale(ship_direction, BULLET_SPEED), ship->velocity);
              bullet[i].lifetime = 0.0f;
              break;
          }
      }
      bullet[i].position = Vector2Add(bullet[i].position, Vector2Scale(bullet[i].velocity, dt));
      bullet[i].lifetime += dt;
      if (bullet[i].lifetime > 2.0) {
          bullet[i].active = 0;
      }
  }
}

