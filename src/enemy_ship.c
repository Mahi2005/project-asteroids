#include "enemy_ship.h"
#include "raymath.h"

void InitEnemyShip(EnemyShip_T *enemy, const char *texturePath, int screenWidth, int screenHeight) {
    enemy->position = (Vector2){ GetRandomValue(0, screenWidth), GetRandomValue(50, screenHeight / 4) };
    enemy->speed = (Vector2){ (float)GetRandomValue(-150, 150), 0 };
    enemy->radius = 35.0f;
    enemy->active = false;
    enemy->shootTimer = 0.0f;
    enemy->spawnTimer = 0.0f;

    enemy->texture = LoadTexture(texturePath);

    for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
        enemy->bullets[i].active = false;
    }
}

void UpdateEnemyShip(EnemyShip_T *enemy, Vector2 playerPos, int screenWidth, int screenHeight) {
    float deltaTime = GetFrameTime();

   
    if (!enemy->active) {
        enemy->spawnTimer += deltaTime;
        if (enemy->spawnTimer >= 10.0f) {
            enemy->active = true;
            enemy->spawnTimer = 0.0f;
            enemy->position = (Vector2){ GetRandomValue(0, screenWidth), GetRandomValue(50, screenHeight / 4) };
            enemy->speed = (Vector2){ (float)GetRandomValue(-150, 150), 0 };
        }
        
    }
    else{

    
    enemy->position.x += enemy->speed.x * deltaTime;

    if (enemy->position.x > screenWidth + enemy->radius) enemy->position.x = -enemy->radius;
    if (enemy->position.x < -enemy->radius) enemy->position.x = screenWidth + enemy->radius;

    
    enemy->shootTimer += deltaTime;
    if (enemy->shootTimer >= 2.0f) { 
        enemy->shootTimer = 0.0f;
        for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
            if (!enemy->bullets[i].active) {
                
                Vector2 dir = Vector2Subtract(playerPos, enemy->position);
                dir = Vector2Normalize(dir);

                
                Vector2 noseOffset = Vector2Scale(dir, 25.0f); 
                enemy->bullets[i].position = Vector2Add(enemy->position, noseOffset);

                enemy->bullets[i].speed = Vector2Scale(dir, 300.0f); // Bullet Speed
                enemy->bullets[i].active = true;
                break;
            }
            }
        }
    }

    
    for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
        if (enemy->bullets[i].active) {
            enemy->bullets[i].position.x += enemy->bullets[i].speed.x * deltaTime;
            enemy->bullets[i].position.y += enemy->bullets[i].speed.y * deltaTime;

            if (enemy->bullets[i].position.x < 0 || enemy->bullets[i].position.x > screenWidth ||
                enemy->bullets[i].position.y < 0 || enemy->bullets[i].position.y > screenHeight) {
                enemy->bullets[i].active = false;
            }
        }
    }
}


void DrawEnemyShip(EnemyShip_T enemy) {
    if (enemy.active){

    
    Vector2 drawPos = { 
        enemy.position.x - (float)enemy.texture.width / 2.0f, 
        enemy.position.y - (float)enemy.texture.height / 2.0f 
    };

    DrawTextureV(enemy.texture, drawPos, WHITE);
}

    
    for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
        if (enemy.bullets[i].active) {
            DrawCircleV(enemy.bullets[i].position, 4.0f, RED);
        }
    }
}


bool CheckBulletHitEnemy(Vector2 bulletPos, EnemyShip_T *enemy) {
    if (!enemy->active) return false;

    if (CheckCollisionCircles(bulletPos, 3.0f, enemy->position, enemy->radius)) {
        enemy->active = false;
        return true;
    }
    return false;
}
// Check if Enemy Bullets hit the Player Ship
bool CheckEnemyBulletHitPlayer(EnemyShip_T *enemy, Ship_T *ship) {
    if (!ship->intact) return false;

    for (int i = 0; i < MAX_ENEMY_BULLETS; i++) {
        if (enemy->bullets[i].active) {
            
            if (CheckCollisionCircles(enemy->bullets[i].position, 10.0f, ship->centroid, ship->radius)) {
                enemy->bullets[i].active = false;
                ship->intact = false;             
                return true;
            }
        }
    }
    return false;
}


void UnloadEnemyShip(EnemyShip_T *enemy) {
    UnloadTexture(enemy->texture);
}