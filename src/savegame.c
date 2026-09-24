#include <stdio.h>
#include <stdlib.h>
#include "savegame.h"

#define SAVE_GAME_FILE "savegame.dat"

bool savegame_exists(void){
    FILE *file = fopen(SAVE_GAME_FILE, "rb");
    if (file == NULL) return false;
    fclose(file);
    return true;
}

void savegame_write(Ship_T *ship, Bullet_T bullets[], Asteroid_T asteroids[], int max_asteroids, 
                    int init_asteroids, int asteroids_count, int total_score, int lives, int level){
    FILE *file = fopen(SAVE_GAME_FILE, "wb");
    if(file == NULL) return;
    fwrite(ship, sizeof(Ship_T), 1, file);
    fwrite(bullets, sizeof(Bullet_T), MAX_BULLETS, file);
    fwrite(&max_asteroids, sizeof(int), 1, file);
    fwrite(&init_asteroids, sizeof(int), 1, file);
    fwrite(&asteroids_count, sizeof(int), 1, file);
    fwrite(&total_score,sizeof(int), 1, file);
    fwrite(&lives, sizeof(int), 1, file);
    fwrite(&level, sizeof(int), 1, file);
    fwrite(asteroids, sizeof(Asteroid_T), max_asteroids, file);

    fclose(file);
}

bool savegame_load(Ship_T *ship, Bullet_T bullets[], Asteroid_T **asteroids, int *max_asteroids,
                    int *init_asteroids, int *asteroids_count, int *total_score, int *lives, int *level){
    FILE *file = fopen(SAVE_GAME_FILE, "rb");
    if(file == NULL) return false;

    fread(ship, sizeof(Ship_T), 1, file);
    fread(bullets, sizeof(Bullet_T), MAX_BULLETS, file);
    fread(max_asteroids, sizeof(int), 1, file);
    fread(init_asteroids, sizeof(int), 1, file);
    fread(asteroids_count, sizeof(int), 1, file);
    fread(total_score,sizeof(int), 1, file);
    fread(lives, sizeof(int), 1, file);
    fread(level, sizeof(int), 1, file);
    *asteroids = calloc(*max_asteroids, sizeof(Asteroid_T));
    fread(*asteroids, sizeof(Asteroid_T), *max_asteroids, file);

    fclose(file);
    return true;
}

void savegame_delete(void){
    remove(SAVE_GAME_FILE);
}

