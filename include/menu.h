#ifndef MENU_H
#define MENU_H
#include "raylib.h"
#include <stdbool.h>
#include "highscores.h"
#include "settings.h"

typedef enum {
    MENU,
    PLAY,
    SETTINGS,
    HIGH_SCORES,
    TUTORIAL,
    CREDITS
} game_screen;

typedef enum {
    BUTTON_CONTINUE,
    BUTTON_NEWGAME,
    BUTTON_PLAY,
    BUTTON_SETTINGS,
    BUTTON_HIGHSCORES,
    BUTTON_TUTORIAL,
    BUTTON_CREDITS,
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
bool draw_settings_menu(int screenwidth, int screenheight, Texture2D back_texture);
bool draw_tutorial_screen(int screenwidth, int screenheight, Texture2D back_texture);
void draw_credits_button(void);
bool is_credits_button_clicked(void);
bool draw_credits_screen(int screenwidth, int screenheight);

#endif