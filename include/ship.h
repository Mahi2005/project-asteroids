#ifndef SHIP_H
#define SHIP_H

#include "raylib.h"

#define EXPLOSION_FRAMES 8

extern int lives;
extern int level;

typedef struct {
    Vector2 position;
    int current_frame;
    float frame_timer;
    float frame_duration;
    int total_frames;       
    int frame_width;        
    int frame_height;
    bool active;
} Explosion_T;

typedef struct {
    Vector2 centroid;
    Vector2 top;
    Vector2 left;
    Vector2 right;
    float radius;
    int bitcodes[3];
    Vector2 velocity;
    float rotation;
    float rotation_speed;
    Vector2 acceleration;
    float max_speed;
    bool intact;
} Ship_T;

void Ship_reposition(Ship_T*, Vector2, float);
void Ship_copy(Ship_T*, Ship_T*);
// Vector2 farthest_vertex_from_bottom(Ship_T *, int);
// Vector2 farthest_vertex_from_top(Ship_T *, int);
// Vector2 farthest_vertex_from_right(Ship_T *, int);
// Vector2 farthest_vertex_from_left(Ship_T *, int);
void Ship_init_vertex_codes(Ship_T* ship, int screen_w, int screen_h);
//void ship_init_assets(texture2D)




int Ship_is_partially_crossed(Ship_T* ship);
int Ship_is_fully_crossed(Ship_T* ship);
void Ship_init(Ship_T* ship);
void Ship_move(Ship_T* ship);
void Ship_screen_wraparound(Ship_T *ship, int screen_w, int screen_h);
void Ship_destroy(Ship_T *ship, Explosion_T *explosion);


void Explosion_update(Explosion_T *explosion, float delta_time);
void Explosion_draw(const Explosion_T *explosion, Ship_T *ship, Texture2D texture);

#endif
