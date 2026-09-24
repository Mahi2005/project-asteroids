#ifndef SAVEGAME_H
#define SAVEGAME_H

#include "ship.h"
#include "bullets.h"
#include "asteroids.h"
#include <stdbool.h>

bool savegame_exists(void);
bool savegame_load(Ship_T *ship, Bullet_T bullets[], Asteroid_T **asteroids, int *max_asteroids,
                    int *init_asteroids, int *asteroid_count, int *total_score, int *lives, int *level);
void savegame_write(Ship_T *ship, Bullet_T bullets[], Asteroid_T asteroids[], int max_asteroids, 
                    int init_asteroids, int asteroids_count, int total_score, int lives, int level);
void savegame_delete(void);
#endif
