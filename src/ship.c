#include "ship.h"
#include "utils.h"




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

void move_ship(Ship *ship) {
    float dt = GetFrameTime();
    if (IsKeyDown(KEY_LEFT) || IsKeyDown(KEY_A)) {
      // ship.rotation -= ship.rotation_speed * dt;
      ship->acceleration = Vector2Rotate(ship->acceleration, -ship->rotation);
      // ship.velocity = Vector2Rotate(ship.velocity, -ship.rotation);
      reposition_ship(ship, Vector2Zero(), -ship->rotation);
    }
    if (IsKeyDown(KEY_RIGHT) || IsKeyDown(KEY_D)) {
      // ship->rotation += ship->rotation_speed*dt;
      ship->acceleration = Vector2Rotate(ship->acceleration, ship->rotation);
      // ship->velocity = Vector2Rotate(ship->velocity, ship->rotation);
      reposition_ship(ship, Vector2Zero(), ship->rotation);
    }

    // float anglerad=(ship->rotation-90.0f)*DEG2RAD;

    if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) {
      // ship->velocity = Vector2Add(ship->velocity, Vector2Scale(ship->top, dt *
      // ship->acceleration));
      Vector2 displacement;
      if (Vector2Length(ship->velocity) < ship->max_speed) {
        Vector2 delta_v = Vector2Scale(ship->acceleration, dt);
        ship->velocity = Vector2Add(ship->velocity, delta_v);
      }
      displacement = Vector2Scale(ship->velocity, dt);
      reposition_ship(ship, displacement, 0);
    } else {
      float damping = expf(-1.2 * dt);
      ship->velocity = Vector2Scale(ship->velocity, damping);
      Vector2 displacement = Vector2Scale(ship->velocity, dt);
      reposition_ship(ship, displacement, 0);
    }
}

void ship_screen_wraparound(Ship* ship, Ship* ship_cpy, int screenwidth, int screenheight) {      
    if (((ship->top.y < 0) || (ship->left.y < 0)) || (ship->right.y < 0)) {
        ship_cpy->centroid =
            Vector2Add(ship->centroid, Vector2Create(0, screenheight));
        ship_cpy->top = Vector2Add(ship->top, Vector2Create(0, screenheight));
        reposition_ship(ship_cpy, Vector2Zero(), 0);
        ship_cpy->velocity = ship->velocity;
        ship_cpy->rotation = ship->rotation;
        ship_cpy->rotation_speed = ship->rotation_speed;
        ship_cpy->acceleration = ship->acceleration;
        ship_cpy->max_speed = ship->max_speed;
        ship_cpy->intact = ship->intact;
    } else if (((ship->top.y > screenheight) || (ship->left.y > screenheight)) ||
               (ship->right.y > screenheight)) {
        ship_cpy->centroid =
            Vector2Add(ship->centroid, Vector2Create(0, -screenheight));
        ship_cpy->top = Vector2Add(ship->top, Vector2Create(0, -screenheight));
        reposition_ship(ship_cpy, Vector2Zero(), 0);
        ship_cpy->velocity = ship->velocity;
        ship_cpy->rotation = ship->rotation;
        ship_cpy->rotation_speed = ship->rotation_speed;
        ship_cpy->acceleration = ship->acceleration;
        ship_cpy->max_speed = ship->max_speed;
        ship_cpy->intact = ship->intact;
    }
    if (((ship->top.x < 0) || (ship->left.x < 0)) || (ship->right.x < 0)) {
        ship_cpy->centroid =
            Vector2Add(ship->centroid, Vector2Create(screenwidth, 0));
        ship_cpy->top = Vector2Add(ship->top, Vector2Create(screenwidth, 0));
        reposition_ship(ship_cpy, Vector2Zero(), 0);
        ship_cpy->velocity = ship->velocity;
        ship_cpy->rotation = ship->rotation;
        ship_cpy->rotation_speed = ship->rotation_speed;
        ship_cpy->acceleration = ship->acceleration;
        ship_cpy->max_speed = ship->max_speed;
        ship_cpy->intact = ship->intact;
    } else if (((ship->top.x > screenwidth) || (ship->left.x > screenwidth)) ||
               (ship->right.x > screenwidth)) {
        ship_cpy->centroid = Vector2Add(ship->centroid, Vector2Create(-screenwidth, 0));
        ship_cpy->top = Vector2Add(ship->top, Vector2Create(-screenwidth, 0));
        reposition_ship(ship_cpy, Vector2Zero(), 0);
        ship_cpy->velocity = ship->velocity;
        ship_cpy->rotation = ship->rotation;
        ship_cpy->rotation_speed = ship->rotation_speed;
        ship_cpy->acceleration = ship->acceleration;
        ship_cpy->max_speed = ship->max_speed;
        ship_cpy->intact = ship->intact;
    }
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
