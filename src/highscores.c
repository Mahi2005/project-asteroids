#include <stdio.h>
#include <string.h>
#include "highscores.h"

void load_highscores(HighScore_T scores[NUM_OF_SCORES]){
    FILE *file = fopen("highscores.txt", "r");
    if (file == NULL){
        for(int i = 0; i < NUM_OF_SCORES; i++) {
            scores[i].score = 0;
            scores[i].name[0] = '\0';
        }
        return;
    }
    for(int i = 0; i < NUM_OF_SCORES; i++){
        if (fscanf(file, "%d %15s", &scores[i].score, scores[i].name) != 2) {
            scores[i].score = 0;
            scores[i].name[0] = '\0';
        }
    }
    fclose(file);
}

void save_highscores(HighScore_T scores[NUM_OF_SCORES]){
    FILE *file = fopen("highscores.txt", "w");
    if (file == NULL) return;
    for(int i = 0; i < NUM_OF_SCORES; i++) {
        fprintf(file, "%d %s\n", scores[i].score, scores[i].name);
    }
    fclose(file);
}

bool highscore_check(HighScore_T scores[NUM_OF_SCORES], int new_score){
    for(int i = 0; i < NUM_OF_SCORES; i++){
        if(new_score > scores[i].score) return true;
    }
    return false;
}

void insert_highscore(HighScore_T scores[NUM_OF_SCORES], int new_score, const char *name){
    for(int i = 0; i < NUM_OF_SCORES; i++){
        if(new_score > scores[i].score){
            for (int j = NUM_OF_SCORES - 1; j > i; j--){
                scores[j] = scores[j - 1];
            }
            scores[i].score = new_score;
            strncpy(scores[i].name, name, NAME_MAX_LEN - 1);
            scores[i].name[NAME_MAX_LEN - 1] = '\0';
            break;
        }
    }
}