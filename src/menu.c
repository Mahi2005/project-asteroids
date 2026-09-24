#include <string.h>
#include <stdio.h>
#include "menu.h"
#define MAX_MENU_BUTTONS 5

int menu_starting_y = 0;
Rectangle menu_buttons[MAX_MENU_BUTTONS];
button menu_button_ids[MAX_MENU_BUTTONS];
const char *menu_button_labels[MAX_MENU_BUTTONS];
int menu_button_count = 0;

Rectangle gameover_buttons[2];
const char *gameover_button_name[2] = { "PLAY AGAIN", "MAIN MENU"};
int button_selected = -1;



bool buttonClicked(Rectangle rectangle, bool is_selected) {

    Vector2 mouse = GetMousePosition();
    bool hovered = CheckCollisionPointRec(mouse, rectangle);
    bool mouse_clicked = hovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    bool key_clicked = (hovered || is_selected) && (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER));
    return mouse_clicked || key_clicked;
}



void draw_button(Rectangle rectangle, const char *label, bool is_selected) {

    Vector2 mouse = GetMousePosition();
    bool hovered = CheckCollisionPointRec(mouse, rectangle) || is_selected;
    Color button_color = hovered ? YELLOW : WHITE;
    DrawRectangleLinesEx(rectangle, 2, button_color);

    int font_size = 20;
    int text_width = MeasureText(label, font_size);

    DrawText(label, rectangle.x + rectangle.width / 2 - text_width / 2, rectangle.y + rectangle.height / 2 - font_size / 2, font_size, button_color);
}

void menu_init(int screenwidth, int screenheight, bool has_saved_game) {

    int button_width = 250;
    int button_height = 50;
    int gap = 20;
    menu_button_count = 0;

    if(has_saved_game){
        menu_button_ids[menu_button_count] = BUTTON_CONTINUE;
        menu_button_labels[menu_button_count] = "CONTINUE";
        menu_button_count++;

        menu_button_ids[menu_button_count] = BUTTON_NEWGAME;
        menu_button_labels[menu_button_count] = "NEW GAME";
        menu_button_count++;
    } else {
        menu_button_ids[menu_button_count] = BUTTON_PLAY;
        menu_button_labels[menu_button_count] = "PLAY";
        menu_button_count++;
    }

    menu_button_ids[menu_button_count] = BUTTON_SETTINGS;
    menu_button_labels[menu_button_count] = "SETTINGS";
    menu_button_count++;

    menu_button_ids[menu_button_count] = BUTTON_HIGHSCORES;
    menu_button_labels[menu_button_count] = "HIGH SCORES";
    menu_button_count++;

    menu_button_ids[menu_button_count] = BUTTON_EXIT;
    menu_button_labels[menu_button_count] = "EXIT";
    menu_button_count++;


    int starting_y = screenheight / 2 - (menu_button_count * (button_height + gap)) / 2;
    menu_starting_y = starting_y;

    for (int i = 0; i < menu_button_count; i++) {
        menu_buttons[i] = (Rectangle){
            .x = screenwidth / 2.0 - button_width / 2.0,
            .y = starting_y + i * (button_height + gap),
            .width = button_width,
            .height = button_height
        };
    }
    button_selected = -1;
}

int update_menu(int screenwidth, int screenheight) {

    if (IsKeyPressed(KEY_DOWN)) {
        button_selected = (button_selected + 1) % menu_button_count;
    }

    if (IsKeyPressed(KEY_UP)) {
        button_selected = (button_selected - 1 + menu_button_count) % menu_button_count;
    }
    Vector2 mouse = GetMousePosition();

    for (int i = 0; i < menu_button_count; i++) {
        if (CheckCollisionPointRec(mouse, menu_buttons[i])) {
            button_selected = i;
        }
    }

    // int clicked = -1;
    // for (int i = 0; i < BUTTON_COUNT; i++) {

    //     if (buttonClicked(menu_buttons[i])) {
    //         clicked = i;
    //         break;
    //     }
    // }

    // if (clicked == -1 && IsKeyPressed(KEY_ENTER)) {
    //     clicked = button_selected;
    // }
    int clicked = -1;
    for (int i = 0; i < menu_button_count; i++) {
        if (buttonClicked(menu_buttons[i], button_selected == i)) {
            clicked = i;
            break;
        }
    }

    const char *title = "ASTEROIDS";
    int title_size = 50;

    DrawText(title, screenwidth / 2 - MeasureText(title, title_size) / 2, menu_starting_y - 80, title_size, WHITE);

    for (int i = 0; i < menu_button_count; i++) {
        draw_button(menu_buttons[i], menu_button_labels[i], button_selected == i);
    }
    if(clicked == -1) return -1;
    return menu_button_ids[clicked];
}

bool menu_draw_todo_screen(const char *title) {

    int screenwidth = GetScreenWidth();
    int screenheight = GetScreenHeight();
    int title_size = 30;

    const char *prompt = "TODO. Press ENTER";

    int prompt_size = 20;

    DrawText(title, screenwidth / 2 - MeasureText(title, title_size) / 2, screenheight / 2 - 30, title_size, WHITE);

    DrawText(prompt, screenwidth / 2 - MeasureText(prompt, prompt_size) / 2, screenheight / 2 + 30, prompt_size, RED);

    return IsKeyPressed(KEY_ENTER);
}

gameOverOption draw_gameOver(int screenwidth, int screenheight, int score) {
    DrawRectangle(0, 0, screenwidth, screenheight,(Color){0, 0, 0, 160});

    const char *game_over = "GAME OVER";

    char score_arr[64];

    int title_size = 40;
    int score_size = 30;

    sprintf(score_arr, "Final score: %d", score);

    DrawText(game_over, screenwidth / 2 - MeasureText(game_over, title_size) / 2, screenheight / 2 - 100, title_size, RED);

    DrawText(score_arr, screenwidth / 2 - MeasureText(score_arr, score_size) / 2, screenheight / 2 - 40, score_size, BLUE);

    int button_width = 220;
    int button_height = 50;
    int gap = 20;
    int start_y = screenheight / 2 + 200;

    gameover_buttons[0] = (Rectangle){
        .x = screenwidth / 2.0 - button_width / 2.0,
        .y = start_y,
        .width = button_width,
        .height = button_height
    };

    gameover_buttons[1] = (Rectangle){
        .x = screenwidth / 2.0 - button_width / 2.0,
        .y = start_y + button_height + gap,
        .width = button_width,
        .height = button_height
    };

    for (int i = 0; i < 2; i++) {

        draw_button(gameover_buttons[i], gameover_button_name[i], false);
    }

    // if (buttonClicked(gameover_buttons[0])) {
    //     return PLAY_AGAIN;
    // }


    // if (buttonClicked(gameover_buttons[1])) {
    //     return MAIN_MENU;
    // }
    if (buttonClicked(gameover_buttons[0], false)) {
        return PLAY_AGAIN;
    }

    if (buttonClicked(gameover_buttons[1], false)) {
        return MAIN_MENU;
    }

    return GAMEOVER;
}

bool draw_back_btn(Texture2D back_texture){
    float size = 40;
    Rectangle back_btn = {20 , 20, size, size};
    Vector2 mouse = GetMousePosition();
    bool hovered = CheckCollisionPointRec(mouse,back_btn);
    Color tint = hovered ? YELLOW : WHITE;

    Rectangle source = {0, 0, (float)back_texture.width, (float)back_texture.height};
    DrawTexturePro(back_texture, source, back_btn, (Vector2){0,0}, 0.0, tint);
    bool mouse_clicked = hovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    bool key_clicked = IsKeyPressed(KEY_BACKSPACE);
    return mouse_clicked || key_clicked;
}

bool menu_draw_highscores(int screenwidth, int screenheight, int scores[], int count, Texture2D back_texture){
    char *title = "HIGH SCORES";
    int title_size = 30;
    DrawText(title, screenwidth / 2 - MeasureText(title, title_size) / 2, screenheight / 2 - 150, title_size, WHITE);

    int score_size = 24;
    int starting_y = screenheight / 2 - 80;
    char line[64];

    for(int i = 0; i < count; i++){
        if(scores[i] <= 0) break;
        sprintf(line, "%d. %d", i + 1, scores[i]);
        DrawText(line, screenwidth / 2 - MeasureText(title, title_size) / 2, starting_y, score_size, RAYWHITE);
        starting_y += score_size + 15;
    }

    return draw_back_btn(back_texture);
}
