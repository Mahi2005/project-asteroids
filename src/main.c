#include "asteroids.h"
#include "bullets.h"
#include "enemy_ship.h"
#include "highscores.h"
#include "menu.h"
#include "raylib.h"
#include "raymath.h"
#include "savegame.h"
#include "ship.h"
#include <limits.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#define DEBUG 1
#define INIT_ASTEROIDS 6
#define MAX_ASTEROIDS INIT_ASTEROIDS * 4

int lives = 3;
int max_asteroids = MAX_ASTEROIDS;
int init_asteroids = INIT_ASTEROIDS;
int asteroids_count = INIT_ASTEROIDS;
int total_score = 0;
int level = 0;
int buffer = 200;

static const int screenwidth = 1300.0;
static const int screenheight = 750.0;

static game_screen current_screen = MENU;

static Ship_T ship;
static EnemyShip_T enemy;
static Texture2D enemy_texture;
static Texture2D ship_texture;
static Texture2D thruster_texture;
static Texture2D back_texture;
static Texture2D pause_texture;
// Global sound and music variables
static Music menu_music;
static Music game_music;
Sound shoot_sound;
Sound exp_sound;
// static Ship_T ship_cpy;
static Bullet_T bullets[MAX_BULLETS];
static Asteroid_T *asteroids;

static HighScore_T high_scores[NUM_OF_SCORES];
static bool is_entering_name = false;
static char name_buffer[NAME_MAX_LEN] = "";
static bool score_recorded = false;
static bool is_paused = false;
static bool should_exit = false;

// static Asteroid_T* asteroids_2;
static bool level_up = false;

static float level_wait_time = 2.5;
static float ship_reinit_wait_time = 1.0;
float ship_invuln_time = 3.0;
bool ship_invuln_flag = true;

void Game_init() {
    SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_RESIZABLE);

    InitWindow(screenwidth, screenheight, "Game");
    InitAudioDevice();
    game_music = LoadMusicStream("assets/gamemusic.mp3");
    shoot_sound = LoadSound("assets/laserShoot.wav");
    exp_sound = LoadSound("assets/explosion.wav");
    PlayMusicStream(game_music);
    ship_texture = LoadTexture("assets/ship.png");
    thruster_texture = LoadTexture("assets/ship_thruster.png");
    back_texture = LoadTexture("assets/back_btn.png");
    pause_texture = LoadTexture("assets/pause_btn.png");

    InitEnemyShip(&enemy, "assets/enemy.png", screenwidth, screenheight);
    SetTargetFPS(60);
    // menu initialization
    bool has_saved_game = savegame_exists();
    menu_init(screenwidth, screenheight, has_saved_game);

    load_highscores(high_scores);

    // screenwidth += buffer;
    // screenheight += buffer;

    // Vector2 screen_center = {.x = screenwidth / 2.0, .y = screenheight
    // / 2.0};
    // Ship initialization
    Ship_init(&ship);
    // Ship_init(&ship_cpy);

    // Asteroids initialization
    asteroids = calloc(max_asteroids, sizeof(Asteroid_T));
    // asteroids_2 = calloc(max_asteroids, sizeof(Asteroid_T));

    SetRandomSeed(time(0));
    int rand_seeds[max_asteroids];
    for (int i = 0; i < max_asteroids; i++) {
        rand_seeds[i] = GetRandomValue(0, INT_MAX);
    }
    for (int i = 0; i < init_asteroids; i++) {
        SetRandomSeed(rand_seeds[i]);
        Asteroid_rand_init(&asteroids[i]);
    }
}

void Game_state_reinit(int level) {
    // if level = 0, this means to restart the game
    // otherwise it implies levelling up, and thus increasing difficulting and
    // unlocking other features which is to be implemented here

    // Clean up the previous asteroids and their copies

    // asteroids = NULL;
    // asteroids_2 = NULL;

    if (level == 0) {
        lives = 3;
        total_score = 0;
        init_asteroids = INIT_ASTEROIDS;
        max_asteroids = MAX_ASTEROIDS;
    }

    int asteroids_increment = 0;
    float velocity_multiplier = 1;

    switch (level) {
    case 0:
        asteroids_increment = 0;
        velocity_multiplier = 1;
        break;
    case 1:
        asteroids_increment = 5;
        velocity_multiplier = 1.3;
        break;
    default:
        asteroids_increment = 5;
        velocity_multiplier = 1.3;
    }
    init_asteroids += asteroids_increment;
    max_asteroids = init_asteroids * 4;

    asteroids_count = init_asteroids;
    if (asteroids != NULL) {
        free(asteroids);
    }

    asteroids = calloc(max_asteroids, sizeof(Asteroid_T));
    // asteroids_2 = realloc(asteroids_2, sizeof(Asteroid_T) * max_asteroids);

    Ship_init(&ship);
    // Ship_init(&ship_cpy);

    SetRandomSeed(time(0));
    int rand_seeds[max_asteroids];
    for (int i = 0; i < max_asteroids; i++) {
        rand_seeds[i] = GetRandomValue(0, INT_MAX);
    }
    for (int i = 0; i < init_asteroids; i++) {
        SetRandomSeed(rand_seeds[i]);
        Asteroid_rand_init(&asteroids[i]);
        asteroids[i].velocity =
            Vector2Scale(asteroids[i].velocity, velocity_multiplier);
    }
}

void Game_draw_menu() {
    BeginDrawing();
    if (current_screen == MENU) {
        ClearBackground(BLACK);

        int choice = update_menu(screenwidth, screenheight);

        if (choice == BUTTON_CONTINUE) {
            savegame_load(&ship, bullets, &asteroids, &max_asteroids,
                          &init_asteroids, &asteroids_count, &total_score,
                          &lives, &level);
            current_screen = PLAY;
        } else if (choice == BUTTON_NEWGAME) {
            savegame_delete();
            level = 0;
            Game_state_reinit(level);
            current_screen = PLAY;
            menu_init(screenwidth, screenheight, false);
        } else if (choice == BUTTON_PLAY) {
            current_screen = PLAY;
        } else if (choice == BUTTON_SETTINGS) {
            current_screen = SETTINGS;
        } else if (choice == BUTTON_HIGHSCORES) {
            current_screen = HIGH_SCORES;
        } else if (choice == BUTTON_EXIT) {
            exit(EXIT_SUCCESS);
        }
    } else if (current_screen == SETTINGS) {

        ClearBackground(BLACK);
        bool back = menu_draw_todo_screen("SETTINGS");
        if (back)
            current_screen = MENU;
        // continue;
    } else if (current_screen == HIGH_SCORES) {

        ClearBackground(BLACK);
        bool back = menu_draw_highscores(screenwidth, screenheight, high_scores,
                                         NUM_OF_SCORES, back_texture);

        if (back)
            current_screen = MENU;
        // continue;
    } else if (current_screen == PLAY) {
        EndDrawing();
        return;
    }
    /*     Game_update(); */

    /*     Game_draw_frame(); */
    /* } */
    EndDrawing();
}

void Game_draw_frame() {
    char debug_info[1000];
    BeginDrawing();
    ClearBackground(BLACK);
    /* if (current_screen == MENU) { */
    /*     Game_draw_menu(); */
    /* } */
    /* else { */
    if (DEBUG) {
        sprintf(debug_info,
                "Speed: %f\n"
                "Max number of asteroids: %d\n"
                "Number of asteroids active: %d\n"
                "Ship invulnerability timer: %f\n",
                Vector2Length(ship.velocity), max_asteroids, asteroids_count,
                ship_invuln_time);
        DrawText(debug_info, 410, 55, 12, RED);
    }
    // draw_score
    char score[10];
    sprintf(score, "%d", total_score);
    DrawText(score, 10, 10, 30, BLUE);

    // draw_lives
    char str_lives[4];
    strcpy(str_lives, (lives == 3)
                          ? "AAA"
                          : ((lives == 2) ? "AA" : ((lives == 1) ? "A" : "")));
    DrawText(str_lives, 100, 10, 30, RED);

    // draw level
    char str_level[10];
    sprintf(str_level, "%d", level);
    DrawText(str_level, 200, 10, 30, GREEN);

    for (int i = 0; i < max_asteroids; i++) {
        if (asteroids[i].state) {
            Asteroid_draw(&asteroids[i], WHITE);
            /* if (Asteroid_is_partially_crossed(&asteroids[i])) { */
            /*     Asteroid_draw(&asteroids_2[i], WHITE); */
            /* } */
            /* if (Asteroid_is_fully_crossed(&asteroids[i])) { */
            /*     // asteroids[i] = asteroids_2[i]; */
            /*     Asteroid_copy(&asteroids[i], &asteroids_2[i]); */
            /* } */
        } else {
            // DrawLineStrip(asteroids[i].vertices, asteroids[i].n_vertices,
            // RED); DrawText("DESTROYED", asteroids[i].position.x,
            // asteroids[i].position.y, 10, YELLOW); draw_explosion_effect;
        }
    }

    if (ship.intact) {
        Vector2 direction = Vector2Subtract(ship.top, ship.centroid);
        float ship_angle = atan2f(direction.y, direction.x) * RAD2DEG + 90.0f;
        // float scale = (ship.radius * 2.5) / ship_texture.width;
        // Vector2 texture_size = {ship_texture.width * scale,
        // ship_texture.height * scale}; Vector2 draw_position =
        // {ship.centroid.x - texture_size.x / 2.0, ship.centroid.y -
        // texture_size.y / 2.0}; DrawTextureEx(ship_texture, draw_position,
        // ship_angle, scale, WHITE);
        float ship_width = ship.radius * 2.5;
        float ship_height = ship.radius * 2.5;

        if (IsKeyDown(KEY_UP) || IsKeyDown(KEY_W)) {
            float thruster_width = ship_width * 0.35;
            float thruster_height = ship_height * 0.5;
            float back_offset = ship_height * 0.35;
            float side_offset = ship_width * 0.28;

            Vector2 local_left = {-back_offset, -side_offset};
            Vector2 local_right = {-back_offset, side_offset};
            float rotation = (ship_angle - 90.0) * DEG2RAD;
            Vector2 world_left = Vector2Rotate(local_left, rotation);
            Vector2 world_right = Vector2Rotate(local_right, rotation);
            Vector2 thruster_left_pos = Vector2Add(ship.centroid, world_left);
            Vector2 thruster_right_pos = Vector2Add(ship.centroid, world_right);

            Rectangle thruster_source = {0, 0, (float)thruster_texture.width,
                                         (float)thruster_texture.height};
            Rectangle thruster_target_left = {thruster_left_pos.x,
                                              thruster_left_pos.y,
                                              thruster_width, thruster_height};
            Rectangle thruster_target_right = {thruster_right_pos.x,
                                               thruster_right_pos.y,
                                               thruster_width, thruster_height};
            Vector2 thruster_origin = {thruster_width / 2.0, 0};

            DrawTexturePro(thruster_texture, thruster_source,
                           thruster_target_left, thruster_origin, ship_angle,
                           WHITE);
            DrawTexturePro(thruster_texture, thruster_source,
                           thruster_target_right, thruster_origin, ship_angle,
                           WHITE);
        }
        Rectangle source_rect = {0, 0, (float)ship_texture.width,
                                 (float)ship_texture.height};
        Rectangle target_rect = {ship.centroid.x, ship.centroid.y, ship_width,
                                 ship_height};
        Vector2 ship_center = {ship_width / 2.0, ship_height / 2.0};
        DrawTexturePro(ship_texture, source_rect, target_rect, ship_center,
                       ship_angle, WHITE);
        // DrawTriangleLines(ship.top, ship.left, ship.right, WHITE);
        if (DEBUG)
            DrawCircleLinesV(ship.centroid,
                             Vector2Distance(ship.top, ship.centroid), YELLOW);
        // if (is_fully_crossed_vert || is_fully_crossed_hor) {
        //    ship = ship_cpy;
        //    DrawTriangleLines(ship.top, ship.left, ship.right, WHITE);
        //}
        /* if (Ship_is_partially_crossed(&ship)) { */
        /*     DrawTriangleLines(ship_cpy.top, ship_cpy.left, ship_cpy.right,
         * WHITE); */
        /* } */
        /* if (Ship_is_fully_crossed(&ship)) { */
        /*     Ship_copy(&ship, &ship_cpy); */
        /* } */
        for (int i = 0; i < MAX_BULLETS; i++) {
            if (bullets[i].active) {
                DrawCircleV(bullets[i].position, 4, RED);
            }
        }
        DrawEnemyShip(enemy);

        if (asteroids_count == 0) {
            char level_msg[100];
            sprintf(level_msg, "LEVEL %d CLEARED", level);
            DrawText(level_msg,
                     screenwidth / 2 - MeasureText("LEVEL CLEARED", 20) / 2,
                     screenheight / 2 - 20, 20, GREEN);
        }
    }

    if (lives > 0) {
        if (draw_pause_btn(pause_texture)) {
            is_paused = !is_paused;
        }
    }

    if (is_paused) {
        pauseOption option = draw_pause_menu(screenwidth, screenheight);
        if (option == PAUSE_CONTINUE)
            is_paused = false;
        else if (option == PAUSE_RESTART) {
            savegame_delete();
            level = 0;
            Game_state_reinit(level);
            is_paused = false;
        } else if (option == PAUSE_MAINMENU) {
            savegame_write(&ship, bullets, asteroids, max_asteroids,
                           init_asteroids, asteroids_count, total_score, lives,
                           level);
            current_screen = MENU;
            is_paused = false;
            menu_init(screenwidth, screenheight, true);
        } else if (option == PAUSE_EXIT) {
            savegame_write(&ship, bullets, asteroids, max_asteroids,
                           init_asteroids, asteroids_count, total_score, lives,
                           level);
            should_exit = true;
        }
    }

    EndDrawing();
}

void Game_update() {
    if (is_paused)
        return;

    for (int i = 0; i < max_asteroids; i++) {
        if (asteroids[i].state) {
            Asteroid_move(&asteroids[i]);
            Asteroid_init_vertex_codes(&asteroids[i], screenwidth + buffer,
                                       screenheight + buffer);
            // Asteroid_init_vertex_codes(&asteroids_new_level[i], screenwidth,
            // screenheight);
            Asteroid_screen_wraparound(&asteroids[i], screenwidth + buffer,
                                       screenheight + buffer);
            // Asteroid_screen_wraparound(&asteroids_new_level[i],
            // &asteroids_new_level_2[i], &asteroids_new_level_3[i],
            // &asteroids_new_level_4[i], screenwidth, screenheight); if (level
            // > 1) {
            //    asteroids[i] = asteroids_new_level[i];
            //    asteroids_2[i] = asteroids_new_level_2[i];
            //    asteroids_3[i] = asteroids_new_level_3[i];
            //    asteroids_4[i] = asteroids_new_level_4[i];
            //};
            // Asteroid_screen_wraparound(&asteroids[i], &asteroids_2[i],
            // &asteroids_3[i], &asteroids_4[i], screenwidth, screenheight);
            // Asteroid_move(&asteroids_2[i]);
        } // else {
        //   Asteroid_delete(asteroids, i);
        //}
    }
    Asteroid_track_count(asteroids);

    UpdateEnemyShip(&enemy, ship.centroid, screenwidth, screenheight);
    // Ship
    if (ship.intact) {
        Ship_move(&ship);
        Ship_init_vertex_codes(&ship, screenwidth + buffer,
                               screenheight + buffer);
        Ship_screen_wraparound(&ship, screenwidth + buffer,
                               screenheight + buffer);
        // Ship_move(&ship_cpy);

        if (asteroids_count == 0) {
            level_wait_time -= GetFrameTime();
            if (level_wait_time <= 0) {
                level_up = true;
                level_wait_time = 2.5;
            }
            if (ship_invuln_time > 0)
                ship_invuln_time -= GetFrameTime();
        }

        if (lives == 3) {
            ship_invuln_time = 0;
        } else {
            ship_invuln_time -= GetFrameTime();
        }
    } else if (lives > 0) {
        for (int i = 0; i < MAX_BULLETS; i++) {
            bullets[i].active = 0;
        }
        // DrawDestroyedShip(&ship);
        ship_reinit_wait_time -= GetFrameTime();
        if (ship_reinit_wait_time <= 0) {
            Ship_init(&ship);
            if (ship_invuln_flag) { // flag is set to true when ship is
                                    // destroyed by Asteroid_strike_ship
                                    // function
                ship_invuln_time = 2.0;
                ship_invuln_flag = false;
            }
            ship_invuln_time -= GetFrameTime();
            ship_reinit_wait_time = 1.0;
        }
    } else {
        if (!score_recorded && !is_entering_name) {
            is_entering_name = highscore_check(high_scores, total_score);
            if (!is_entering_name) {
                score_recorded = true;
                savegame_delete();
            }
        }

        if (is_entering_name) {
            bool confirmed =
                draw_name_entry(screenwidth, screenheight, total_score,
                                name_buffer, NAME_MAX_LEN);
            if (confirmed) {
                insert_highscore(high_scores, total_score, name_buffer);
                save_highscores(high_scores);
                is_entering_name = false;
                score_recorded = true;
                savegame_delete();
                name_buffer[0] = '\0';
            }
        } else {
            gameOverOption option =
                draw_gameOver(screenwidth, screenheight, total_score);
            if (option == PLAY_AGAIN) {
                level = 0;
                Game_state_reinit(level);
                score_recorded = false;
            } else if (option == MAIN_MENU) {
                level = 0;
                current_screen = MENU;
                Game_state_reinit(level);
                score_recorded = false;
                menu_init(screenwidth, screenheight, savegame_exists());
            }
        }
    }

    if (lives > 0) {
        // Bullets
        Bullet_shoot(bullets, &ship);
        Bullet_screen_wraparound(bullets, screenwidth + buffer,
                                 screenheight + buffer);

        // Collision detection, and corresponding fragment or destruct effects
        Bullet_strike_asteroids(bullets, asteroids);
        bool was_intact = ship.intact;
        Asteroid_strike_ship(asteroids, &ship);
        if (was_intact && !ship.intact) {
            PlaySound(exp_sound);
        }
        // Collision detection, and corresponding fragment or destruct effects
        Bullet_strike_asteroids(bullets, asteroids);

        for (int i = 0; i < MAX_BULLETS; i++) {
            if (bullets[i].active) {
                if (CheckBulletHitEnemy(bullets[i].position, &enemy)) {
                    total_score += 500;
                    bullets[i].active = false;
                    PlaySound(exp_sound);
                }
            }
        }
        if (ship.intact && ship_invuln_time <= 0) {
            if (CheckEnemyBulletHitPlayer(&enemy, &ship)) {
                lives--;
                ship_invuln_flag = true;
                PlaySound(exp_sound);
            }
        }
        was_intact = ship.intact;
        Asteroid_strike_ship(asteroids, &ship);
        if (was_intact && !ship.intact) {
            PlaySound(exp_sound);
        }
    }
}

int main(void) {
    Game_init();
    while ((!WindowShouldClose() || IsKeyPressed(KEY_R)) && !should_exit) {
        UpdateMusicStream(game_music);
        if ((current_screen == MENU) || (current_screen == SETTINGS) ||
            (current_screen == HIGH_SCORES)) {
            Game_draw_menu();
        } else if (current_screen == PLAY) {
            if (level_up) {
                level++;
                Game_state_reinit(level);
                level_up = false;
            }
            Game_draw_frame();
            Game_update();
        }
    }

    if (current_screen == PLAY && lives > 0) {
        savegame_write(&ship, bullets, asteroids, max_asteroids, init_asteroids,
                       asteroids_count, total_score, lives, level);
    } else {
        savegame_delete();
    }

    UnloadSound(shoot_sound);
    UnloadSound(exp_sound);

    UnloadMusicStream(game_music);
    CloseAudioDevice();
    UnloadTexture(ship_texture);
    UnloadTexture(thruster_texture);
    UnloadTexture(back_texture);
    UnloadTexture(pause_texture);

    UnloadEnemyShip(&enemy);
    CloseWindow();
    return 0;
}
