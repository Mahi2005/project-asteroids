#ifndef MENU_H
#define MENU_H
#include "raylib.h"
#include <stdbool.h>
#include "highscores.h"

typedef enum {
    MENU,
    PLAY,
    SETTINGS,
    HIGH_SCORES
} game_screen;

typedef enum {
    BUTTON_CONTINUE,
    BUTTON_NEWGAME,
    BUTTON_PLAY,
    BUTTON_SETTINGS,
    BUTTON_HIGHSCORES,
    BUTTON_EXIT
} button;

typedef enum {
    GAMEOVER = -1,
    PLAY_AGAIN,
    MAIN_MENU
} gameOverOption;

typedef enum {
    PAUSE_NONE = -1,
    PAUSE_CONTINUE,
    PAUSE_RESTART,
    PAUSE_MAINMENU,
    PAUSE_EXIT
} pauseOption;

void menu_init(int screenwidth, int screenheight, bool has_saved_game);
int update_menu(int screenwidth, int screenheight);
bool menu_draw_todo_screen(const char* title);
gameOverOption draw_gameOver(int screenwidth, int screenheight, int score);
bool menu_draw_highscores(int screenwidth, int screenheight, HighScore_T scores[], int count, Texture2D back_texture);
bool draw_name_entry(int screenwidth, int screenheight, int score, char *name_buffer, int max_len);
bool draw_pause_btn(Texture2D pause_texture);
pauseOption draw_pause_menu(int screenwidth, int screenheight);

#endif