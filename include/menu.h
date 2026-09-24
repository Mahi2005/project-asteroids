#ifndef MENU_H
#define MENU_H
#include "raylib.h"

typedef enum {
    MENU,
    PLAY,
    SETTINGS,
    HIGH_SCORES
} game_screen;

typedef enum {
    BUTTON_PLAY,
    BUTTON_SETTINGS,
    BUTTON_HIGHSCORES,
    BUTTON_EXIT,
    BUTTON_COUNT
} button;

typedef enum {
    GAMEOVER = -1,
    PLAY_AGAIN,
    MAIN_MENU
} gameOverOption;

void menu_init(int screenwidth, int screenheight);
int update_menu(int screenwidth, int screenheight);
bool menu_draw_todo_screen(const char* title);
gameOverOption draw_gameOver(int screenwidth, int screenheight, int score);
bool menu_draw_highscores(int screenwidth, int screenheight, int scores[], int count, Texture2D back_texture);


#endif