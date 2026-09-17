#include <math.h>
#include <stdbool.h>
#include <stdio.h>
#include <limits.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include "raylib.h"
#include "raymath.h"
#include "ship.h"
#include "bullets.h"
#include "utils.h"
#include "asteroids.h"
#include "menu.h"


#define DEBUG 0

int lives = 3;
int asteroids_count = INIT_ASTEROIDS;
int total_score = 0;
int level = 1;



int main(void) {

    game_screen current_screen = MENU;
    
    Ship_T ship;
    Ship_T ship_cpy;
    Bullet_T bullets[MAX_BULLETS] = {0};
    Asteroid_T asteroids[MAX_ASTEROIDS] = {0};
    Asteroid_T asteroids_2[MAX_ASTEROIDS] = {0};
    
    const int screenwidth = 800.0;
    const int screenheight = 600.0;
    
    InitWindow(screenwidth, screenheight, "Game");

    // menu initialization
    menu_init(screenwidth, screenheight);
    
    int buffer = 0;
    // screenwidth += buffer;
    // screenheight += buffer;
    
    Vector2 screen_center = {.x = screenwidth / 2.0, .y = screenheight / 2.0};
    
    // Ship initialization
    Ship_init(&ship);
    Ship_init(&ship_cpy);
    
    // Asteroids initialization
    SetRandomSeed(time(0));
    int rand_seeds[MAX_ASTEROIDS];
    for (int i = 0; i < MAX_ASTEROIDS; i++) {
        rand_seeds[i] = GetRandomValue(0, INT_MAX);
    }
    for (int i = 0; i < INIT_ASTEROIDS; i++) {
        SetRandomSeed(rand_seeds[i]);
        Asteroid_rand_init(&asteroids[i]);
    }
    
    SetTargetFPS(60);
    
    float wait_time = 0.0f;
    
    while (!WindowShouldClose() || IsKeyPressed(KEY_R)) {
        if (current_screen == MENU) {
            BeginDrawing();
            ClearBackground(BLACK);
            
            int choice = update_menu(screenwidth, screenheight);
            
            EndDrawing();
            
            if (choice == BUTTON_PLAY)
                {
                    current_screen = PLAY;
                }
            else if (choice == BUTTON_SETTINGS)
                {
                    current_screen = SETTINGS;
                }
            else if (choice == BUTTON_HIGHSCORES)
                {
                    current_screen = HIGH_SCORES;
                }
            else if (choice == BUTTON_EXIT)
                {
                    break;
                }
            continue;
        }
        
        if (current_screen == SETTINGS){
            BeginDrawing();
            
            ClearBackground(BLACK);
            bool back = menu_draw_todo_screen("SETTINGS");
            EndDrawing();
            if (back)
                current_screen = MENU;
            continue;
        }
        
     if (current_screen == HIGH_SCORES){
         BeginDrawing();
         
         ClearBackground(BLACK);
         bool back = menu_draw_todo_screen("HIGH SCORES");
         EndDrawing();
         
         if (back) current_screen = MENU;
         continue;
    }
     if (current_screen == PLAY) {
         for(int i = 0; i < MAX_ASTEROIDS; i++) {
             if (asteroids[i].state) {
                 Asteroid_move(&asteroids[i]);
                 Asteroid_init_vertex_codes(&asteroids[i], screenwidth + buffer, screenheight + buffer);
                 // Asteroid_init_vertex_codes(&asteroids_new_level[i], screenwidth, screenheight);
                 Asteroid_screen_wraparound(&asteroids[i], &asteroids_2[i], screenwidth + buffer, screenheight + buffer);
                 // Asteroid_screen_wraparound(&asteroids_new_level[i], &asteroids_new_level_2[i], &asteroids_new_level_3[i], &asteroids_new_level_4[i], screenwidth, screenheight);          
                 // if (level > 1) {
                 //    asteroids[i] = asteroids_new_level[i];
                 //    asteroids_2[i] = asteroids_new_level_2[i];
                 //    asteroids_3[i] = asteroids_new_level_3[i];
                 //    asteroids_4[i] = asteroids_new_level_4[i];
                 //};
                 // Asteroid_screen_wraparound(&asteroids[i], &asteroids_2[i], &asteroids_3[i], &asteroids_4[i], screenwidth, screenheight);
                 Asteroid_move(&asteroids_2[i]);
             }// else {
             //   Asteroid_delete(asteroids, i);
             //}
            }
         // Ship
         if (ship.intact) {
             Ship_move(&ship);
             Ship_init_vertex_codes(&ship, screenwidth + buffer, screenheight + buffer);
             Ship_screen_wraparound(&ship, &ship_cpy, screenwidth + buffer, screenheight + buffer);
             Ship_move(&ship_cpy);
         }
         
         // Bullets
         Bullet_shoot(bullets, &ship);
         Bullet_screen_wraparound(bullets, screenwidth + buffer, screenheight + buffer);
         
         
         // Collision detection, and corresponding fragment or destruct effects
         Bullet_strike_asteroids(bullets, asteroids, asteroids_2);
         Asteroid_strike_ship(asteroids, asteroids_2, &ship);
         if (asteroids_count == 0) {
             
         }
         
         char debug_info[1000];
         BeginDrawing();
         ClearBackground(BLACK);
         
         if (DEBUG) {
             sprintf(debug_info,
                     "Speed: %f\n"
                     "Max number of asteroids: %d\n"
                     "Number of asteroids active: %d\n",
                     Vector2Length(ship.velocity),
                     MAX_ASTEROIDS,
                     asteroids_count);
             DrawText(debug_info, 410, 55, 12, RED);
         }
         // draw_score
         char score[10];
         sprintf(score, "%d\n", total_score);
         DrawText(score, 10, 10, 30, BLUE);
         
         // draw_lives
         char str_lives[3];
         strcpy(str_lives, (lives == 3) ? "AAA" : ((lives == 2) ? "AA" : ((lives == 1) ? "A" : "")));
         DrawText(str_lives, 100, 10, 30, RED);
         
         for (int i = 0; i < MAX_ASTEROIDS; i++) {
             if (asteroids[i].state) {
                 Asteroid_draw(&asteroids[i], WHITE);
                 if (Asteroid_is_partially_crossed(&asteroids[i])) {
                     Asteroid_draw(&asteroids_2[i], WHITE);
                 }
                 if (Asteroid_is_fully_crossed(&asteroids[i])) {
                     // asteroids[i] = asteroids_2[i];
                     Asteroid_copy(&asteroids[i], &asteroids_2[i]);
                 }
             } else {
                 // DrawLineStrip(asteroids[i].vertices, asteroids[i].n_vertices, RED);
                 // DrawText("DESTROYED", asteroids[i].position.x, asteroids[i].position.y, 10, YELLOW);
                 // draw_explosion_effect;
             }
         }
         
         if (ship.intact) {
             DrawTriangleLines(ship.top, ship.left, ship.right, WHITE);
             if (DEBUG) DrawCircleLinesV(ship.centroid, Vector2Distance(ship.top, ship.centroid), YELLOW);
             // if (is_fully_crossed_vert || is_fully_crossed_hor) {
             //    ship = ship_cpy;
             //    DrawTriangleLines(ship.top, ship.left, ship.right, WHITE);
             //}
             if (Ship_is_partially_crossed(&ship)) {
                 DrawTriangleLines(ship_cpy.top, ship_cpy.left, ship_cpy.right, WHITE);
             }
             if (Ship_is_fully_crossed(&ship)) {
                 Ship_copy(&ship, &ship_cpy);
             }
             for (int i = 0; i < MAX_BULLETS; i++) {
                 if (bullets[i].active) {
                     DrawCircleV(bullets[i].position, 4, RED);
                 }
             }
             
             if (asteroids_count == 0) {
                 char level_msg[100];
                 // wait_time = 2.5;
                 // wait_time -= GetFrameTime();
                 // while (wait_time > 0) {
                 sprintf(level_msg, "LEVEL %d CLEARED", level);
                 DrawText(level_msg, screenwidth / 2 - MeasureText("LEVEL CLEARED", 20) / 2, screenheight / 2 - 20, 20, GREEN);
                 // }
                 // if (wait_time <= 0) {
                 //    level++;
                 //    asteroids_count = INIT_ASTEROIDS + 2;
                 
             }
         } else if (lives > 0) {
             for (int i = 0; i < MAX_BULLETS; i++) {
                 bullets[i].active = 0;
             }
             // DrawDestroyedShip(&ship);
             wait_time -= GetFrameTime();
             if (wait_time <= 0) {
                 ship.intact = true;
                 Ship_init(&ship);
                 wait_time = 0;
             }
             
         } else {               
             // for (int i = 0; i < MAX_ASTEROIDS; i++) {
             //    asteroids[i].state = 0;
             //}
             // DrawText("GAME OVER!", screenwidth / 2 - MeasureText("LEVEL CLEARED", 20) / 2, screenheight / 2 - 20, 20, RED);
             gameOverOption option = draw_gameOver(screenwidth, screenheight, total_score);
             
             if (option == PLAY_AGAIN) {
                 lives = 3;
                 total_score = 0;
                 level = 1;
                 asteroids_count = INIT_ASTEROIDS + 3;
                 
                 for (int i = 0; i < MAX_BULLETS; i++) {
                     bullets[i].active = 0;
                 }
                 
                 Ship_init(&ship);
                 Ship_init(&ship_cpy);
                 
                 SetRandomSeed(time(0));
                 
                 for (int i = 0; i < asteroids_count; i++) {
                     Asteroid_rand_init(&asteroids[i]);
                 }
                 current_screen = PLAY;
                 wait_time = 0.0f;
             }
             
             else if (option == MAIN_MENU) {
                 lives = 3;
                 total_score = 0;
                 level = 1;
                 asteroids_count = INIT_ASTEROIDS;
                 
                 for (int i = 0; i < MAX_BULLETS; i++) {
                     bullets[i].active = 0;
                 }
                 
                 Ship_init(&ship);
                 Ship_init(&ship_cpy);
                 
                 current_screen = MENU;
                 wait_time = 0.0f;
             }
         }
         
         // DrawCircle(ship.centroid.x, ship.centroid.y, 5, RED);
         EndDrawing();   
     }
    }
    CloseWindow();
}
