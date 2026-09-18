#include <string.h>
#include <stdio.h>
#include "menu.h"

Rectangle menu_buttons[BUTTON_COUNT];
const char *button_name[BUTTON_COUNT] = {"PLAY", "SETTINGS", "HIGH SCORES", "EXIT"};
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

void menu_init(int screenwidth, int screenheight) {

    int button_width = 250;
    int button_height = 50;
    int gap = 20;
    int starting_y = screenheight / 2 - 50;

    for (int i = 0; i < BUTTON_COUNT; i++) {
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
        button_selected = (button_selected + 1) % BUTTON_COUNT;
    }

    if (IsKeyPressed(KEY_UP)) {
        button_selected = (button_selected - 1 + BUTTON_COUNT) % BUTTON_COUNT;
    }
    Vector2 mouse = GetMousePosition();

    for (int i = 0; i < BUTTON_COUNT; i++) {
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
    for (int i = 0; i < BUTTON_COUNT; i++) {
        if (buttonClicked(menu_buttons[i], button_selected == i)) {
            clicked = i;
            break;
        }
    }

    const char *title = "ASTEROIDS";
    int title_size = 50;

    DrawText(title, screenwidth / 2 - MeasureText(title, title_size) / 2, screenheight / 2 - 200, title_size, WHITE);

    for (int i = 0; i < BUTTON_COUNT; i++) {
        draw_button(menu_buttons[i], button_name[i], button_selected == i);
    }
    return clicked;
}

bool menu_draw_todo_screen(const char *title) {

    int screenwidth = GetScreenWidth();
    int screenheight = GetScreenHeight();
    int title_size = 30;

    const char *prompt = "TODO. Press ENTER";

    int prompt_size = 20;

    DrawText(title,
             screenwidth / 2 - MeasureText(title, title_size) / 2, screenheight / 2 - 30, title_size, WHITE);

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
