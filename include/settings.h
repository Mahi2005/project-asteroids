#ifndef SETTINGS_H
#define SETTINGS_H
#include <stdbool.h>

typedef enum {
    DIF_EASY,
    DIF_MEDIUM,
    DIF_HARD
} Difficulty;

typedef struct {
    float music_volume;
    float sound_volume;
    bool muted;
    Difficulty difficulty;
} Settings_T;

typedef struct {
    int enemy_count_level1;
    int enemy_count_increment;
    float enemy_spawn_interval;
    int asteroid_increment;
    float asteroid_velocity_multiplier;
} DifficultyPar_T;

extern Settings_T settings;

void settings_load(void);
void settings_save(void);
DifficultyPar_T settings_get_difficulty_parameters(void);
int settings_get_enemy_count(int level);

#endif

