#ifndef HIGHSCORES_H
#define HIGHSCORES_H
#include <stdbool.h>

#define NUM_OF_SCORES 3
#define NAME_MAX_LEN 16

typedef struct {
    int score;
    char name[NAME_MAX_LEN];
} HighScore_T;

void load_highscores(HighScore_T scores[NUM_OF_SCORES]);
void save_highscores(HighScore_T scores[NUM_OF_SCORES]);
bool highscore_check(HighScore_T scores[NUM_OF_SCORES], int new_score);
void insert_highscore(HighScore_T scores[NUM_OF_SCORES], int new_score, const char *name);

#endif