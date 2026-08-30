#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include "raylib.h"
#include "raymath.h"
#include "ship.h"
#include "bullets.h"
#include "utils.h"
#include "asteroids.h"





int main(void) {
  Ship ship;
  Ship ship_cpy;
  Bullets bullet[MAX_BULLETS] = {0};
  const int screenwidth = 800.0;
  const int screenheight = 600.0;
  InitWindow(screenwidth, screenheight, "Game");

  Vector2 screen_center = {.x = screenwidth / 2.0, .y = screenheight / 2.0};

  // Ship initialization
  float ship_length = 50;
  init_ship(&ship, ship_length);

  SetTargetFPS(60);
  while (!WindowShouldClose() || IsKeyPressed(KEY_R)) {
      move_ship(&ship);
      ship_screen_wraparound(&ship, &ship_cpy, screenwidth, screenheight);
      
      // Bullets
      shoot_bullets(bullet, &ship);
      bullets_screen_wraparound(bullet, screenwidth, screenheight);

      // Collision detection & effects
      // bullet_destroy_ship(bullet, &ship);
      
      Vector2 far_vertex_bot = farthest_vertex_from_bottom(&ship, screenheight);
      Vector2 far_vertex_top = farthest_vertex_from_top(&ship, screenheight);
      Vector2 far_vertex_right = farthest_vertex_from_right(&ship, screenwidth);
      Vector2 far_vertex_left = farthest_vertex_from_left(&ship, screenwidth);
      int is_partially_crossed =
          crossed_border_partial(&ship, screenwidth, screenheight);
      int is_fully_crossed_top = (far_vertex_top.y < 0);
      int is_fully_crossed_bot = (far_vertex_bot.y > screenheight);
      int is_fully_crossed_left = (far_vertex_left.x < 0);
      int is_fully_crossed_right = (far_vertex_right.x > screenwidth);
      char debug_info[1000];
      BeginDrawing();
      ClearBackground(BLACK);
      
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
