#include <stdio.h>
#include  "settings.h"

Settings_T settings;

void settings_load(void){
    FILE *file = fopen("settings.txt", "r");
    if (file == NULL) {
        settings.music_volume = 0.5;
        settings.sound_volume = 0.5;
        settings.muted = false;
        settings.difficulty = DIF_EASY;
        return;
    }
    int muted_int = 0, diff_int = 0;
    if(fscanf(file, "%f %f %d %d", &settings.music_volume, &settings.sound_volume, &muted_int, &diff_int) != 4){
        settings.music_volume = 0.5;
        settings.sound_volume = 0.5;
        settings.muted = false;
        settings.difficulty = DIF_EASY;
    } else {
        settings.muted = muted_int;
        settings.difficulty = (Difficulty)diff_int;
    }
    fclose(file);
}

void settings_save(void){
    FILE *file = fopen("settings.txt", "w");
    if (file == NULL) return;
    fprintf(file, "%f %f %d %d\n", settings.music_volume, settings.sound_volume, settings.muted ? 1 : 0, (int)settings.difficulty);
    fclose(file);
}

DifficultyPar_T settings_get_difficulty_parameters(void){
    DifficultyPar_T p;
    switch ( (settings.difficulty))
    {
    case DIF_EASY:
        p = (DifficultyPar_T){1, 1, 20.0, 5, 1.3};
        break;
    case DIF_MEDIUM:
        p = (DifficultyPar_T){2, 2, 10.0, 8, 1.6};
        break;
    case DIF_HARD:
        p = (DifficultyPar_T){2, 3, 10.0, 10, 1.9};
        break;
    default:
        p = (DifficultyPar_T){1, 1, 20.0, 5, 1.3};
        break;
    }
    return p;
}

int settings_get_enemy_count(int level){
    DifficultyPar_T p = settings_get_difficulty_parameters();
    int effective_level = (level < 1) ? 1 : level;
    return p.enemy_count_level1 + (effective_level - 1) * p.enemy_count_increment;
}
