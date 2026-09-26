#include <string.h>
#include <stdio.h>
#include "menu.h"
#define MAX_MENU_BUTTONS 6

int menu_starting_y = 0;
Rectangle menu_buttons[MAX_MENU_BUTTONS];
button menu_button_ids[MAX_MENU_BUTTONS];
const char *menu_button_labels[MAX_MENU_BUTTONS];
int menu_button_count = 0;

Rectangle gameover_buttons[2];
const char *gameover_button_name[2] = { "PLAY AGAIN", "MAIN MENU"};
int button_selected = -1;

Rectangle pause_buttons[4];
const char *pause_button_name[4] = {"CONTINUE", "RESTART", "MAIN MENU", "EXIT"};



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

    menu_button_ids[menu_button_count] = BUTTON_TUTORIAL;
    menu_button_labels[menu_button_count] = "TUTORIAL";
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

bool menu_draw_highscores(int screenwidth, int screenheight, HighScore_T scores[], int count, Texture2D back_texture){
    char *title = "HIGH SCORES";
    int title_size = 30;
    DrawText(title, screenwidth / 2 - MeasureText(title, title_size) / 2, screenheight / 2 - 150, title_size, WHITE);

    int score_size = 24;
    int starting_y = screenheight / 2 - 80;
    char line[64];

    for(int i = 0; i < count; i++){
        if(scores[i].score <= 0) break;
        sprintf(line, "%d. %d - %s", i + 1, scores[i].score, scores[i].name);
        DrawText(line, screenwidth / 2 - MeasureText(line, score_size) / 2, starting_y, score_size, RAYWHITE);
        starting_y += score_size + 15;
    }

    return draw_back_btn(back_texture);
}

bool draw_pause_btn(Texture2D pause_texture){
    float size = 40;
    Rectangle pause_button = {GetScreenWidth() - size - 20, 20, size, size};
    Vector2 mouse = GetMousePosition();
    bool hovered = CheckCollisionPointRec(mouse, pause_button);
    Color tint = hovered ? YELLOW : WHITE;

    Rectangle source = {0, 0, (float)pause_texture.width, (float)pause_texture.height};
    DrawTexturePro(pause_texture, source, pause_button, (Vector2){0,0}, 0.0, tint);

    bool mouse_clicked = hovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
    bool key_clicked = IsKeyPressed(KEY_P);
    return mouse_clicked || key_clicked;
}

pauseOption draw_pause_menu(int screenwidth, int screenheight){
    DrawRectangle(0, 0, screenwidth, screenheight, (Color){0, 0, 0, 160});
    const char *title ="PAUSED";
    int title_size = 40;
    DrawText(title, screenwidth / 2 - MeasureText(title,title_size) / 2, screenheight / 2 - 180, title_size, WHITE);
    
    int btn_width = 220;
    int btn_height = 50;
    int gap = 20;
    int starting_y = screenheight / 2 - 70;

    for(int i = 0; i < 4; i++){
        pause_buttons[i] = (Rectangle){
            .x = screenwidth / 2.0 - btn_width / 2.0,
            .y = starting_y + i * (btn_height + gap),
            .width = btn_width,
            .height = btn_height
        };
    }

    for(int i = 0; i < 4; i++){
        draw_button(pause_buttons[i], pause_button_name[i], false);
    }

    if(buttonClicked(pause_buttons[0], false)) return PAUSE_CONTINUE;
    if(buttonClicked(pause_buttons[1], false)) return PAUSE_RESTART;
    if(buttonClicked(pause_buttons[2], false)) return PAUSE_MAINMENU;
    if(buttonClicked(pause_buttons[3], false)) return PAUSE_EXIT;

    return PAUSE_NONE;
}

bool draw_name_entry(int screenwidth, int screenheight, int score, char *name_buffer, int max_len){
    DrawRectangle(0, 0, screenwidth, screenheight, (Color){0, 0, 0, 180});

    const char *title = "NEW HIGH SCORE!";
    int title_size = 36;
    DrawText(title, screenwidth / 2 - MeasureText(title, title_size) / 2, screenheight / 2 - 120, title_size, YELLOW);
    char score_text[32];
    sprintf(score_text, "Score: %d", score);
    DrawText(score_text, screenwidth / 2 - MeasureText(score_text, 24) / 2, screenheight / 2 - 70, 24 , WHITE);

    const char *prompt = "Enter your name:";
    DrawText(prompt, screenwidth / 2 - MeasureText(prompt, 20) / 2, screenheight / 2 - 20, 20, WHITE);

    int len = strlen(name_buffer);
    int ch = GetCharPressed();
    while(ch > 0) {
        if (ch >= 32 && ch <= 125 && len < max_len - 1) {
            name_buffer[len] = (char)ch;
            name_buffer[len + 1] = '\0';
            len++;
        }
        ch = GetCharPressed();
    }
    if(IsKeyPressed(KEY_BACKSPACE) && len > 0){
        name_buffer[len - 1] = '\0';
    }

    Rectangle box = {screenwidth / 2 - 150, screenheight / 2 + 20, 300, 40};
    DrawRectangleLinesEx(box, 2, WHITE);
    DrawText(name_buffer, box.x + 10, box.y + 10, 20, YELLOW);

    const char *confirm = "Press ENTER to confirm";
    DrawText(confirm, screenwidth / 2 - MeasureText(confirm, 16) / 2, screenheight / 2 + 80, 16, GRAY);
    return IsKeyPressed(KEY_ENTER) && len > 0;
}

static bool music_dragging = false;
static bool sound_dragging = false;

static float update_slider(Rectangle track, float value, bool *dragging, bool left_clicked, bool right_clicked) {
    Vector2 mouse = GetMousePosition();
    float handle_x = track.x + track.width * value;
    Rectangle handle_hitbox = {handle_x - 10, track.y - 10, 20, 30};

    if(IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mouse, handle_hitbox)){
        *dragging = true;
    }
    if(IsMouseButtonReleased(MOUSE_BUTTON_LEFT)){
        *dragging = false;
    }

    if(*dragging){
        float pos = (mouse.x - track.x) / track.width;
        if(pos < 0) pos = 0;
        if(pos > 1) pos = 1;
        value = pos;
    } else if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && CheckCollisionPointRec(mouse, track)) {
        float pos = (mouse.x - track.x) / track.width;
        if(pos < 0) pos = 0;
        if(pos > 1) pos = 1;
        value = pos;
    }

    if(left_clicked) value -= 0.1;
    if(right_clicked) value += 0.1;
    if(value < 0) value = 0;
    if(value > 1) value = 1;

    return value;
}

static void draw_slider_visual(Rectangle track, float value){
    DrawRectangleRec(track, GRAY);
    DrawRectangleLinesEx(track, 1, WHITE);
    Rectangle fill = {track.x, track.y, track.width * value, track.height};
    DrawRectangleRec(fill, YELLOW);
    float handle_x = track.x + track.width * value;
    Vector2 handle_center = {handle_x, track.y + track.height / 2};
    DrawCircleV(handle_center, 10, WHITE);
}

bool draw_settings_menu(int screenwidth, int screenheight, Texture2D back_texture){
    const char *title = "SETTINGS";
    int title_size = 34;
    DrawText(title, screenwidth / 2 - MeasureText(title,title_size) / 2, screenheight / 2 - 220, title_size, WHITE);

    int slider_width = 300;
    int slider_x = screenwidth / 2 - slider_width / 2;

    DrawText("MUSIC VOLUME", slider_x, screenheight / 2 - 160, 18, WHITE);
    Rectangle music_left_arrow = {slider_x - 40, screenheight / 2 - 140, 30, 30};
    Rectangle music_track ={slider_x, screenheight / 2 - 135, slider_width, 10};
    Rectangle music_right_arrow = {slider_x + slider_width + 10, screenheight / 2 - 140, 30, 30};
    draw_button(music_left_arrow, "<", false);
    draw_button(music_right_arrow, ">", false);
    bool music_left = buttonClicked(music_left_arrow, false);
    bool music_right = buttonClicked(music_right_arrow, false);
    settings.music_volume = update_slider(music_track, settings.music_volume, &music_dragging, music_left, music_right);
    draw_slider_visual(music_track, settings.music_volume);

    DrawText("SOUND VOLUME", slider_x, screenheight / 2 - 80, 18, WHITE);
    Rectangle sound_left_arrow = {slider_x - 40, screenheight / 2 - 60, 30, 30};
    Rectangle sound_track = {slider_x, screenheight / 2 - 55, slider_width, 10};
    Rectangle sound_right_arrow = {slider_x + slider_width + 10, screenheight / 2 - 60, 30, 30};
    draw_button(sound_left_arrow, "<", false);
    draw_button(sound_right_arrow, ">", false);
    bool sound_left = buttonClicked(sound_left_arrow, false);
    bool sound_right = buttonClicked(sound_right_arrow, false);
    settings.sound_volume = update_slider(sound_track, settings.sound_volume, &sound_dragging, sound_left, sound_right);
    draw_slider_visual(sound_track, settings.sound_volume);

    Rectangle mute_btn = {screenwidth / 2 - 100, screenheight / 2 - 10, 200, 40};
    draw_button(mute_btn,settings.muted ? "UNMUTE" : "MUTE", false);
    if(buttonClicked(mute_btn, false)) settings.muted = !settings.muted;

    DrawText("DIFFICULTY", screenwidth / 2 - MeasureText("DIFFICULTY", 18) / 2, screenheight / 2 + 50, 18, WHITE);

    int diff_btn_width = 120;
    int diff_gap = 20;
    int diff_total_width = diff_btn_width * 3 + diff_gap * 2;
    int diff_start_x = screenwidth / 2 - diff_total_width / 2;
    int diff_y = screenheight / 2 + 80;

    Rectangle easy_btn = {diff_start_x, diff_y, diff_btn_width, 40};
    Rectangle medium_btn = {diff_start_x + diff_btn_width + diff_gap, diff_y, diff_btn_width, 40};
    Rectangle hard_btn = {diff_start_x + 2 * (diff_btn_width + diff_gap) , diff_y, diff_btn_width, 40};

    draw_button(easy_btn, "EASY", settings.difficulty == DIF_EASY);
    draw_button(medium_btn, "MEDIUM", settings.difficulty == DIF_MEDIUM);
    draw_button(hard_btn, "HARD", settings.difficulty == DIF_HARD);

    if(buttonClicked(easy_btn,false)) settings.difficulty = DIF_EASY;
    if(buttonClicked(medium_btn, false)) settings.difficulty = DIF_MEDIUM;
    if(buttonClicked(hard_btn, false)) settings.difficulty = DIF_HARD;

    return draw_back_btn(back_texture);
}

bool draw_tutorial_screen(int screenwidth, int screenheight, Texture2D back_texture){
    const char *title = "HOW TO PLAY";
    int title_size = 30;
    DrawText(title, screenwidth / 2 - MeasureText(title, title_size) / 2, screenheight / 2 - 280, title_size, WHITE);

    int text_size = 18;
    int line_gap = 27;
    int y = screenheight / 2 - 220;
    int x = screenwidth / 2 - 260;

    const char *lines[] = {
        "CONTROLS",
        "  W / UP ARROW    - Thrust forward",
        "  A / LEFT ARROW  - Rotate left",
        "  D / RIGHT ARROW - Rotate right",
        "  SPACE           - Shoot",
        "  P               - Pause",
        "",
        "OBJECTIVE",
        "  Destroy asteroids to earn points.",
        "  Large asteroids split into smaller ones when hit.",
        "  Avoid colliding with asteroids and enemy ships.",
        "  Watch out for enemy ships that appear and shoot back.",
        "",
        "NEXT LEVEL",
        "  Destroy every asteroid on screen to clear the level.",
        "  A short pause follows, then the next level begins",
        "  with more asteroids, faster asteroids, and tougher enemies."
    };

    int line_count = sizeof(lines) / sizeof(lines[0]);

    for(int i = 0; i < line_count; i++){
        Color color = (lines[i][0] != '\0' && lines[i][0] != ' ') ? YELLOW : WHITE;
        DrawText(lines[i], x, y, text_size, color);
        y += line_gap;
    }
    return draw_back_btn(back_texture);
}

static float credits_scroll_y = 0;

void draw_credits_button(void){
    Rectangle credits_btn = { 20, GetScreenHeight() - 60, 160, 40};
    Vector2 mouse = GetMousePosition();
    bool hovered = CheckCollisionPointRec(mouse, credits_btn);
    Color color = hovered ? YELLOW : WHITE;
    DrawText("CREDITS", credits_btn.x + 20, credits_btn.y + 10, 20, color);
}

bool is_credits_button_clicked(void){
    Rectangle credits_btn = { 20, GetScreenHeight() - 60, 160, 40};
    Vector2 mouse = GetMousePosition();
    bool hovered = CheckCollisionPointRec(mouse, credits_btn);
    return hovered && IsMouseButtonPressed(MOUSE_BUTTON_LEFT);
}

bool draw_credits_screen(int screenwidth, int screenheight){
    const char *lines[] = {
        "CREDITS",
        "",
        "",
        "SABER HOSSAIN ABIR",
        "Initial ship moving code",
        "Bullets",
        "Sound effects & music",
        "Enemy ship",
        "",
        "S.M. TAZRIAN REZA MAHI",
        "Asteroids shape generation & fragmentation",
        "Ship and asteroids screen wraparound, collision detection",
        "Scoring system and lives",
        "Leveling system",
        "",
        "SHAJIDUL ISLAM FAHIM",
        "Ship texture and thruster effect",
        "Menu system and settings",
        "High score storage and management",
        "Game save and pause mechanism",
        "",
        "",
        "THANK YOU FOR PLAYING"
    };

    int line_count = sizeof(lines) / sizeof(lines[0]);
    int text_size = 24;
    int line_gap = 40;
    float scroll_speed = 40.0;
    credits_scroll_y -= scroll_speed * GetFrameTime();
    float y = screenheight + credits_scroll_y;

    for(int i = 0; i < line_count; i++){
        bool is_name = (i == 3 || i == 9 || i == 15);
        Color color;
        if(i == 0) {
            text_size = 44;
             color = YELLOW;
        } else if(is_name) {
            text_size = 26;
            color = YELLOW;
        } else {
            text_size = 20;
            color = WHITE;
        }

        int text_width = MeasureText(lines[i], text_size);
        DrawText(lines[i], screenwidth / 2 - text_width / 2, (int)y, text_size, color);
        y += line_gap;
    }
    float total_dis = screenheight + line_count * line_gap;

    bool finished_or_back = (-credits_scroll_y >= total_dis) || IsKeyPressed(KEY_BACKSPACE) || IsMouseButtonPressed(MOUSE_BUTTON_LEFT) || IsMouseButtonPressed(MOUSE_BUTTON_RIGHT);

    if(finished_or_back){
        credits_scroll_y = 0;
    }

    return finished_or_back;
}