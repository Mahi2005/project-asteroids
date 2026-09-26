#ifndef ENEMY_SHIP_H
#define ENEMY_SHIP_H
#include "raylib.h"
#include "ship.h"
#include <stdbool.h>
#define MAX_ENEMY_BULLETS 10
typedef struct {
    Vector2 position;
    Vector2 speed;
    bool active;

} EnemyBullet_T;
typedef struct {
    Vector2 position;
    Vector2 speed;
    float radius;
    bool active;
    float shootTimer;
    float spawnTimer;
    EnemyBullet_T bullets[MAX_ENEMY_BULLETS];
    Texture2D texture;
} EnemyShip_T;

void InitEnemyShip(EnemyShip_T *enemy, const char *texturePath, int screenWidth,
                   int screenHeight);
void UpdateEnemyShip(EnemyShip_T *enemy, Vector2 playerPos, int screenWidth,
                     int screenHeight, float spawn_interval);
void DrawEnemyShip(EnemyShip_T enemy);
void UnloadEnemyShip(EnemyShip_T *enemy);

bool CheckBulletHitEnemy(Vector2 bulletPos, EnemyShip_T *enemy);
bool CheckEnemyBulletHitPlayer(EnemyShip_T *enemy, Ship_T *ship);

void ResetEnemyShip(EnemyShip_T *enemy, Texture2D texture, int screenWidth, int screenHeight, float initial_spawn_timer);

#endif
