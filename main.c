#include "raylib.h"
#include "raymath.h"
#include <math.h>
#include <stdbool.h>
#include <stdio.h>

#define MAX_BULLETS 20
#define BULLET_SPEED 400

typedef struct {
  Vector2 centroid;
  Vector2 top;
  Vector2 left;
  Vector2 right;
  Vector2 velocity;
  float rotation;
  float rotation_speed;
  Vector2 acceleration;
  float max_speed;
  bool intact;
} Ship;

typedef struct {
  Vector2 position;
  Vector2 velocity;
  float radius;
  int active;
  float lifetime;
} Bullets;

Vector2 Vector2Create(float x, float y) {
  Vector2 v = {.x = x, .y = y};
  return v;
}

Vector2 Vector2RotateAround(Vector2 point, float angle, Vector2 center) {
  return Vector2Add(Vector2Rotate(Vector2Subtract(point, center), angle),
                    center);
}

Vector2 Vector2ScaleRelative(Vector2 point, float scale, Vector2 center) {
  Vector2 relative = Vector2Subtract(point, center);
  relative = Vector2Scale(relative, scale);
  return Vector2Add(relative, center);
}

void reposition_ship(Ship *ship, Vector2 displacement, float angle) {
  // orients the ship's top, left and right vertices after updating its centroid
  // i.e position by translation or rotation
  // direction of ship is determined by direction of its top vertex w.r.t its
  // centroid
  ship->centroid = Vector2Add(ship->centroid, displacement);
  ship->top = Vector2Add(ship->top, displacement);
  ship->top = Vector2RotateAround(ship->top, angle, ship->centroid);
  ship->right = Vector2ScaleRelative(
      Vector2RotateAround(ship->top, 2 * PI / 3, ship->centroid), 0.5,
      ship->centroid);
  ship->left = Vector2ScaleRelative(
      Vector2RotateAround(ship->top, 4 * PI / 3, ship->centroid), 0.5,
      ship->centroid);
}

Vector2 farthest_vertex_from_bottom(Ship *ship, int screenheight) {
  float a[3]; // order corresponds to top, right, left
  // each element is the perpendicular distance of the corresponding vertex from
  // the bottom screen border
  a[0] = screenheight - ship->top.y;
  a[1] = screenheight - ship->right.y;
  a[2] = screenheight - ship->left.y;
  int farthest_vertex = 0;
  float max_distance = -1;
  for (int i = 0; i < 3; i++) {
    if (a[i] > max_distance) {
      max_distance = a[i];
      farthest_vertex = i;
    }
  }
  switch (farthest_vertex) {
  case 0:
    return ship->top;
  case 1:
    return ship->right;
  case 2:
    return ship->left;
  }
  return Vector2Zero();
}

Vector2 farthest_vertex_from_top(Ship *ship, int screenheight) {
  float a[3]; // order corresponds to top, right, left
  // each element is the perpendicular distance of the corresponding vertex from
  // the bottom screen border
  a[0] = ship->top.y;
  a[1] = ship->right.y;
  a[2] = ship->left.y;
  int farthest_vertex = 0;
  float max_distance = -1;
  for (int i = 0; i < 3; i++) {
    if (a[i] > max_distance) {
      max_distance = a[i];
      farthest_vertex = i;
    }
  }
  switch (farthest_vertex) {
  case 0:
    return ship->top;
  case 1:
    return ship->right;
  case 2:
    return ship->left;
  }
  return Vector2Zero();
}

Vector2 farthest_vertex_from_right(Ship *ship, int screenwidth) {
  float a[3]; // order corresponds to top, right, left
  // each element is the perpendicular distance of the corresponding vertex from
  // the bottom screen border
  a[0] = screenwidth - ship->top.x;
  a[1] = screenwidth - ship->right.x;
  a[2] = screenwidth - ship->left.x;
  int farthest_vertex = 0;
  float max_distance = -1;
  for (int i = 0; i < 3; i++) {
    if (a[i] > max_distance) {
      max_distance = a[i];
      farthest_vertex = i;
    }
  }
  switch (farthest_vertex) {
  case 0:
    return ship->top;
  case 1:
    return ship->right;
  case 2:
    return ship->left;
  }
  return Vector2Zero();
}

Vector2 farthest_vertex_from_left(Ship *ship, int screenwidth) {
  float a[3]; // order corresponds to top, right, left
  // each element is the perpendicular distance of the corresponding vertex from
  // the bottom screen border
  a[0] = ship->top.x;
  a[1] = ship->right.x;
  a[2] = ship->left.x;
  int farthest_vertex = 0;
  float max_distance = -1;
  for (int i = 0; i < 3; i++) {
    if (a[i] > max_distance) {
      max_distance = a[i];
      farthest_vertex = i;
    }
  }
  switch (farthest_vertex) {
  case 0:
    return ship->top;
  case 1:
    return ship->right;
  case 2:
    return ship->left;
  }
  return Vector2Zero();
}

int crossed_border_partial(Ship *ship, int screenwidth, int screenheight) {
  // function for determining if any vertex of the ship has crossed the screen
  // border
  if ((ship->top.x < 0) || (ship->top.x > screenwidth))
    return 1;
  if ((ship->top.y < 0) || (ship->top.y > screenheight))
    return 1;
  if ((ship->right.x < 0) || (ship->right.x > screenwidth))
    return 1;
  if ((ship->right.y < 0) || (ship->right.y > screenheight))
    return 1;
  if ((ship->left.x < 0) || (ship->left.x > screenwidth))
    return 1;
  if ((ship->left.y < 0) || (ship->left.y > screenheight))
    return 1;
  return 0;
}

void init_ship(Ship *ship, int ship_length) {
  float init_speed = 10;
  ship->centroid =
      Vector2Create(GetScreenWidth() / 2.0, GetScreenHeight() / 2.0);
  ship->top = Vector2Add(ship->centroid, Vector2Create(0, -ship_length));
  reposition_ship(ship, Vector2Zero(), 0);
  ship->rotation = (1 / 10.0) * (PI / 6);
  ship->velocity = Vector2Create(0, -init_speed);
  ship->acceleration = Vector2Create(0, -500);
  ship->intact = true;
  ship->max_speed = 800.0;
}

void destruct_ship(Ship *ship) {
  // TODO: Implement ship destruction effects here
  // Main idea is to detach the lines of the ship and show them being rotated or
  // displaced in random directions.
  ship->intact = false;
}

void DrawDestroyedShip(Ship *ship) {
  DrawText("SHIP DESTROYED. GAME OVER!\nPress R to play again",
           ship->centroid.x, ship->centroid.y, 20, RED);
}

int main(void) {
  Ship ship;
  Ship ship_cpy;
  const int screenwidth = 800.0;
  const int screenheight = 600.0;
  InitWindow(screenwidth, screenheight, "Game");

  Vector2 screen_center = {.x = screenwidth / 2.0, .y = screenheight / 2.0};

  // Ship initialization
  float ship_length = 50;
  init_ship(&ship, ship_length);

  SetTargetFPS(60);
  while (!WindowShouldClose() || IsKeyPressed(KEY_R)) {
    float dt = GetFrameTime();
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
      // ship.rotation -= ship.rotation_speed * dt;
      ship.acceleration = Vector2Rotate(ship.acceleration, -ship.rotation);
      // ship.velocity = Vector2Rotate(ship.velocity, -ship.rotation);
      reposition_ship(&ship, Vector2Zero(), -ship.rotation);
    }
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
      // ship.rotation += ship.rotation_speed*dt;
      ship.acceleration = Vector2Rotate(ship.acceleration, ship.rotation);
      // ship.velocity = Vector2Rotate(ship.velocity, ship.rotation);
      reposition_ship(&ship, Vector2Zero(), ship.rotation);
    }

    // float anglerad=(ship.rotation-90.0f)*DEG2RAD;

    if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) {
      // ship.velocity = Vector2Add(ship.velocity, Vector2Scale(ship.top, dt *
      // ship.acceleration));
      Vector2 displacement;
      if (Vector2Length(ship.velocity) < ship.max_speed) {
        Vector2 delta_v = Vector2Scale(ship.acceleration, dt);
        ship.velocity = Vector2Add(ship.velocity, delta_v);
      }
      displacement = Vector2Scale(ship.velocity, dt);
      reposition_ship(&ship, displacement, 0);
    } else {
      float damping = expf(-1.2 * dt);
      ship.velocity = Vector2Scale(ship.velocity, damping);
      Vector2 displacement = Vector2Scale(ship.velocity, dt);
      reposition_ship(&ship, displacement, 0);
    }

    Vector2 far_vertex_bot = farthest_vertex_from_bottom(&ship, screenheight);
    Vector2 far_vertex_top = farthest_vertex_from_top(&ship, screenheight);
    Vector2 far_vertex_right = farthest_vertex_from_right(&ship, screenwidth);
    Vector2 far_vertex_left = farthest_vertex_from_left(&ship, screenwidth);

    if (((ship.top.y < 0) || (ship.left.y < 0)) || (ship.right.y < 0)) {
      ship_cpy.centroid =
          Vector2Add(ship.centroid, Vector2Create(0, screenheight));
      ship_cpy.top = Vector2Add(ship.top, Vector2Create(0, screenheight));
      reposition_ship(&ship_cpy, Vector2Zero(), 0);
      ship_cpy.velocity = ship.velocity;
      ship_cpy.rotation = ship.rotation;
      ship_cpy.rotation_speed = ship.rotation_speed;
      ship_cpy.acceleration = ship.acceleration;
      ship_cpy.max_speed = ship.max_speed;
    } else if (((ship.top.y > screenheight) || (ship.left.y > screenheight)) ||
               (ship.right.y > screenheight)) {
      ship_cpy.centroid =
          Vector2Add(ship.centroid, Vector2Create(0, -screenheight));
      ship_cpy.top = Vector2Add(ship.top, Vector2Create(0, -screenheight));
      reposition_ship(&ship_cpy, Vector2Zero(), 0);
      ship_cpy.velocity = ship.velocity;
      ship_cpy.rotation = ship.rotation;
      ship_cpy.rotation_speed = ship.rotation_speed;
      ship_cpy.acceleration = ship.acceleration;
      ship_cpy.max_speed = ship.max_speed;
    }
    if (((ship.top.x < 0) || (ship.left.x < 0)) || (ship.right.x < 0)) {
      ship_cpy.centroid =
          Vector2Add(ship.centroid, Vector2Create(screenwidth, 0));
      ship_cpy.top = Vector2Add(ship.top, Vector2Create(screenwidth, 0));
      reposition_ship(&ship_cpy, Vector2Zero(), 0);
      ship_cpy.velocity = ship.velocity;
      ship_cpy.rotation = ship.rotation;
      ship_cpy.rotation_speed = ship.rotation_speed;
      ship_cpy.acceleration = ship.acceleration;
      ship_cpy.max_speed = ship.max_speed;
    } else if (((ship.top.x > screenwidth) || (ship.left.x > screenwidth)) ||
               (ship.right.x > screenwidth)) {
      ship_cpy.centroid =
          Vector2Add(ship.centroid, Vector2Create(-screenwidth, 0));
      ship_cpy.top = Vector2Add(ship.top, Vector2Create(-screenwidth, 0));
      reposition_ship(&ship_cpy, Vector2Zero(), 0);
      ship_cpy.velocity = ship.velocity;
      ship_cpy.rotation = ship.rotation;
      ship_cpy.rotation_speed = ship.rotation_speed;
      ship_cpy.acceleration = ship.acceleration;
      ship_cpy.max_speed = ship.max_speed;
    }
    Vector2 ship_direction = Vector2Normalize(ship.acceleration);
    Bullets bullet[20];
    if (IsKeyPressed(KEY_SPACE)) {
      for (int i = 0; i < MAX_BULLETS; i++) {
        if (!bullet[i].active) {
          float shipradius = 50;
          bullet[i].position = Vector2Add(
              ship.centroid, Vector2Scale(ship_direction, shipradius));
          bullet[i].active = 1;
          bullet[i].velocity = Vector2Add(
              Vector2Scale(ship_direction, BULLET_SPEED), ship.velocity);
          bullet[i].lifetime = 0.0f;
          break;
        }
      }
    }
    for (int i = 0; i < MAX_BULLETS; i++) {
      if (bullet[i].active) {
        bullet[i].position = Vector2Add(bullet[i].position,
                                        Vector2Scale(bullet[i].velocity, dt));
        bullet[i].lifetime += dt;
        if (bullet[i].lifetime > 2.0) {
          bullet[i].active = 0;
        }
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
        bool bullet_collision = CheckCollisionPointTriangle(
            bullet[i].position, ship.top, ship.left, ship.right);
        if (bullet_collision) {
          destruct_ship(&ship);
          bullet[i].active = 0;
        }
      }
    }
    // Collision

    BeginDrawing();
    ClearBackground(BLACK);
    int is_partially_crossed =
        crossed_border_partial(&ship, screenwidth, screenheight);
    int is_fully_crossed_top = (far_vertex_top.y < 0);
    int is_fully_crossed_bot = (far_vertex_bot.y > screenheight);
    int is_fully_crossed_left = (far_vertex_left.x < 0);
    int is_fully_crossed_right = (far_vertex_right.x > screenwidth);
    char debug_info[1000];
    sprintf(debug_info,
            "Distance of farthest vertex from bottom: %f\n"
            "Distance of farthest vertex from top: %f\n"
            "Distance of farthest vertex from right: %f\n"
            "Distance of farthest vertex from left: %f\n"
            "Crossed border partially? %s\n"
            "Crossed vert. border fully? %s\n"
            "Crossed hor. border fully? %s\n"
            "Speed: %f\n"
            "Is ship intact: %s\n",
            screenheight - far_vertex_bot.y, far_vertex_top.y,
            screenwidth - far_vertex_right.x, far_vertex_left.x,
            is_partially_crossed ? "yes" : "no",
            (is_fully_crossed_top || is_fully_crossed_bot) ? "yes" : "no",
            (is_fully_crossed_left || is_fully_crossed_right) ? "yes" : "no",
            Vector2Length(ship.velocity), ship.intact ? "yes" : "no");
    DrawText(debug_info, 410, 55, 12, RED);
    if (ship.intact) {
      DrawTriangleLines(ship.top, ship.left, ship.right, WHITE);
      // if (is_fully_crossed_vert || is_fully_crossed_hor) {
      //    ship = ship_cpy;
      //    DrawTriangleLines(ship.top, ship.left, ship.right, WHITE);
      //}
      if (is_partially_crossed) {
        DrawTriangleLines(ship_cpy.top, ship_cpy.left, ship_cpy.right, WHITE);
      }
      if ((is_fully_crossed_left || is_fully_crossed_right) ||
          (is_fully_crossed_bot || is_fully_crossed_top)) {
        ship = ship_cpy;
      }
      for (int i = 0; i < MAX_BULLETS; i++) {
        if (bullet[i].active) {
          DrawCircleV(bullet[i].position, 5.0f, RED);
        }
      }
    } else {
      for (int i = 0; i < MAX_BULLETS; i++) {
        bullet[i].active = 0;
      }
      DrawDestroyedShip(&ship);
      bool R_pressed = IsKeyPressed(KEY_R);
      if (R_pressed) {
        init_ship(&ship, ship_length);
      };
    }
    // DrawCircle(ship.centroid.x, ship.centroid.y, 5, RED);
    EndDrawing();
  }
  CloseWindow();
}
