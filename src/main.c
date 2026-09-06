#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <limits.h>
#include "raylib.h"
#include "raymath.h"
#include "ship.h"
#include "bullets.h"
#include "utils.h"
#include "asteroids.h"
#include <time.h>


static int lives = 3;

int main(void) {
  Ship_T ship;
  Ship_T ship_cpy;
  Bullet_T bullets[MAX_BULLETS] = {0};
  Asteroid_T asteroids[MAX_ASTEROIDS] = {0};
  Asteroid_T asteroids_2[MAX_ASTEROIDS] = {0};
  Asteroid_T asteroids_3[MAX_ASTEROIDS] = {0};
  Asteroid_T asteroids_4[MAX_ASTEROIDS] = {0};
  const int screenwidth = 800.0;
  const int screenheight = 600.0;
  InitWindow(screenwidth, screenheight, "Game");

  Vector2 screen_center = {.x = screenwidth / 2.0, .y = screenheight / 2.0};

  // Ship initialization
  float ship_length = 50;
  init_ship(&ship, ship_length);
  // Asteroids initialization
  SetRandomSeed(time(0));
  
  int rand_seeds[MAX_ASTEROIDS];
  for (int i = 0; i < MAX_ASTEROIDS; i++) {
      rand_seeds[i] = GetRandomValue(0, INT_MAX);
  }
  for (int i = 0; i < MAX_ASTEROIDS; i++) {
      SetRandomSeed(rand_seeds[i]);
      Asteroid_rand_init(&asteroids[i]);
  }
  SetTargetFPS(60);
  while (!WindowShouldClose() || IsKeyPressed(KEY_R)) {
      // Ship
      move_ship(&ship);
      ship_screen_wraparound(&ship, &ship_cpy, screenwidth, screenheight);
      
      // Bullets
      Bullet_shoot(bullets, &ship);
      Bullet_screen_wraparound(bullets, screenwidth, screenheight);
      
      // Collision detection & effects
      // bullet_destroy_ship(bullet, &ship);
      
      // Asteroids

      for(int i = 0; i < MAX_ASTEROIDS; i++) {
          Asteroid_move(&asteroids[i]);
          Asteroid_init_vertex_codes(&asteroids[i], screenwidth, screenheight);
          Asteroid_screen_wraparound(&asteroids[i], &asteroids_2[i], &asteroids_3[i], &asteroids_4[i], screenwidth, screenheight);
          Asteroid_move(&asteroids_2[i]);
          Asteroid_move(&asteroids_3[i]);
          Asteroid_move(&asteroids_4[i]);
      }
      
      // Collision detection, and corresponding fragment or destruct effects
      Bullet_strike_asteroids(bullets, asteroids);
      Asteroid_strike_ship(asteroids, &ship);
      
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
              "Is ship intact: %s\n"
              "Asteroid position: %f, %f\n"
              "Total number of asteroids: %d\n"
              "Asteroid 0 partially crossed?: %s\n",
              screenheight - far_vertex_bot.y, far_vertex_top.y,
              screenwidth - far_vertex_right.x, far_vertex_left.x,
              is_partially_crossed ? "yes" : "no",
              (is_fully_crossed_top || is_fully_crossed_bot) ? "yes" : "no",
              (is_fully_crossed_left || is_fully_crossed_right) ? "yes" : "no",
              Vector2Length(ship.velocity), ship.intact ? "yes" : "no",
              asteroids[0].position.x, asteroids[0].position.y,
              MAX_ASTEROIDS,
              Asteroid_is_partially_crossed(&asteroids[0]) ? "yes" : "no");
      DrawText(debug_info, 410, 55, 12, RED);
      char score[10];
      sprintf(score, "%d\n", total_score);
      DrawText(score, 10, 10, 30, BLUE);
      for (int i = 0; i < MAX_ASTEROIDS; i++) {
          if (asteroids[i].state) {
              Asteroid_draw(&asteroids[i], WHITE);
              if (Asteroid_is_partially_crossed(&asteroids[i])) {
                  Asteroid_draw(&asteroids_2[i], WHITE);
                  Asteroid_draw(&asteroids_3[i], WHITE);
                  Asteroid_draw(&asteroids_4[i], WHITE);
              }
              if (Asteroid_is_fully_crossed(&asteroids[i])) {
                  asteroids[i] = asteroids_2[i];
              }
          } else {
              DrawLineStrip(asteroids[i].vertices, asteroids[i].n_vertices, RED);
              DrawText("DESTROYED", asteroids[i].position.x, asteroids[i].position.y, 10, YELLOW);
          }
      }
      if (ship.intact) {
          DrawTriangleLines(ship.top, ship.left, ship.right, WHITE);
          DrawCircleLinesV(ship.centroid, Vector2Distance(ship.top, ship.centroid) * (1 - 0.2), YELLOW);
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
              if (bullets[i].active) {
                  DrawCircleV(bullets[i].position, 5.0f, RED);
              }
          }
      } else if (lives > 0) {
          for (int i = 0; i < MAX_BULLETS; i++) {
              bullets[i].active = 0;
          }
          DrawDestroyedShip(&ship);
          bool R_pressed = IsKeyPressed(KEY_R);
          if (R_pressed) {
              init_ship(&ship, ship_length);
          };
      } else {
          DrawText("GAME OVER!", screenwidth - 50, screenheight + 20, 20, RED);
      }
      // DrawCircle(ship.centroid.x, ship.centroid.y, 5, RED);
      EndDrawing();
  }
  CloseWindow();
}
