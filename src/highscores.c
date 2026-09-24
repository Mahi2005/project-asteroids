#include <stdio.h>
#include "highscores.h"

void load_highscores(int scores[NUM_OF_SCORES]){
    FILE *file = fopen("highscores.txt", "r");
    if (file == NULL){
        for(int i = 0; i < NUM_OF_SCORES; i++) scores[i] = 0;
        return;
    }
    for(int i = 0; i < NUM_OF_SCORES; i++){
        if (fscanf(file, "%d", &scores[i]) != 1) scores[i] = 0;
    }
    fclose(file);
}

void save_highscores(int scores[NUM_OF_SCORES]){
    FILE *file = fopen("highscores.txt", "w");
    if (file == NULL) return;
    for(int i = 0; i < NUM_OF_SCORES; i++) fprintf(file, "%d\n", scores[i]);
    fclose(file);
}

void check_highscores(int scores[NUM_OF_SCORES], int new_score){
    for(int i = 0; i < NUM_OF_SCORES; i++){
        if(new_score > scores[i]){
            for(int j = NUM_OF_SCORES - 1; j > i; j--){
                scores[j] = scores[j - 1];
            }
            scores[i] = new_score;
            break;
        }
    }
}