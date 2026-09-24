#ifndef HIGHSCORES_H
#define HIGHSCORES_H

#define NUM_OF_SCORES 3

void load_highscores(int scores[NUM_OF_SCORES]);
void save_highscores(int scores[NUM_OF_SCORES]);
void check_highscores(int scores[NUM_OF_SCORES], int new_score);

#endif