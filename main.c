#include "raylib.h"
#include"raymath.h"
#include<math.h>
#define MAX_BULLETS 20
typedef struct Bullet{
    Vector2 position;
    Vector2 speed;
    bool active;
    float lifetime;
}Bullet;
// typedef struct ship{
//     Vector2 position;
//     Vector2 speed;
//     float acceleration;
//     float rotation;
//     float rotationSpeed;
//     float maxSpeed;
// }ship;


int main(void)
{
    const int screenwidth=800;
    const int screenheight=600;
    InitWindow(screenwidth, screenheight, "Game");
    Vector2 shipPosition={screenwidth/2.0,screenheight/2.0};
    Vector2 shipspeed={0,0};
    float shiprotation=0.0f;
    float rotationspeed=250.0f;
    float acceleration=450.0f;
    //ship ship ={0};
    // ship.position=(Vector2){screenwidth/.0f,screenheight/.0f};
    // ship.speed=(Vector2){0,0};
    // ship.rotation=0.0f;
    // ship.acceleration=350.0f;
    // ship.rotationSpeed=0;
    // ship.maxSpeed=400.0f;
    Bullet bullet[MAX_BULLETS]={0};
    float bulletspeed=550.0f;
    SetTargetFPS(60);
    while (!WindowShouldClose())
    {
        float deltatime=GetFrameTime();
        if(IsKeyDown(KEY_LEFT)||IsKeyDown(KEY_A)) shiprotation-=rotationspeed*deltatime;
        if(IsKeyDown(KEY_RIGHT)||IsKeyDown(KEY_D)) shiprotation+=rotationspeed*deltatime;

        // float anglerad=(ship.rotation-90.0f)*DEG2RAD;
        Vector2 forward={0.0f,-1.0f};
        forward=Vector2Rotate(forward,shiprotation*DEG2RAD);

        if(IsKeyDown(KEY_UP)||IsKeyDown(KEY_W)) 
        {
            shipspeed.x+=forward.x*deltatime*acceleration;
            shipspeed.y+=forward.y*deltatime*acceleration;

        }
        if(IsKeyDown(KEY_DOWN)||IsKeyDown(KEY_S)) 
        {
            shipspeed.x-=forward.x*acceleration*deltatime;
            shipspeed.y-=forward.y*deltatime*acceleration;

        }
        // float currspeed=sqrt(ship.speed.x*ship.speed.x+ship.speed.y+ship.speed.y);
        // if(currspeed>ship.maxSpeed)
        // {
        //     ship.speed.x=(ship.speed.x/currspeed)*ship.maxSpeed;
        //     ship.speed.y=(ship.speed.y/currspeed)*ship.maxSpeed;
        
        float deceleration=powf(.2f,deltatime);
        shipspeed.x*=deceleration;
        shipspeed.y*=deceleration;
        shipPosition.x+=shipspeed.x*deltatime;
        shipPosition.y+=shipspeed.y*deltatime;
        if(shipPosition.x>screenwidth)
        {
            shipPosition.x=0.0f;
        }
        if(shipPosition.y>screenheight)
        {
            shipPosition.y=0.0f;
        }
        if(shipPosition.x<0)
        {
            shipPosition.x=screenwidth;
        }
        if(shipPosition.y<0)
        {
            shipPosition.y=screenheight;
        }
        if(IsKeyPressed(KEY_SPACE))
        {
        for(int i=0;i<MAX_BULLETS;i++)
        {
            if(!bullet[i].active)
            {
                float shipradius=40;
                bullet[i].position.x=shipPosition.x+(forward.x*shipradius);
                bullet[i].position.y=shipPosition.y+(forward.y*shipradius);
                bullet[i].active=1;
                bullet[i].speed.x=forward.x*bulletspeed+shipspeed.x;
                bullet[i].speed.y=forward.y*bulletspeed+shipspeed.y;
                bullet[i].lifetime=0.0f;
                break;
            }
        }
        }
        for(int i=0;i<MAX_BULLETS;i++)
        {
            if(bullet[i].active)
            {
                bullet[i].position.x+=bullet[i].speed.x*deltatime;
                bullet[i].position.y+=bullet[i].speed.y*deltatime;
                bullet[i].lifetime+=deltatime;
                if(bullet[i].lifetime>1.2f)
                {
                    bullet[i].active=false;
                }
                if(bullet[i].position.x>screenwidth)bullet[i].position.x=0.0f;
                if(bullet[i].position.x<0)bullet[i].position.x=screenwidth;
                if(bullet[i].position.y>screenwidth)bullet[i].position.x=0.0f;
                if(bullet[i].position.y<0)bullet[i].position.x=screenheight;

            }
        }

        // for(int i=0;i<MAX_BULLETS;i++)
        // {
        //     while(!bullet[i].active)
        //     {
        //         bullet[i].position.x+=ship.position.x
        //     }
        // }
        BeginDrawing();
        ClearBackground(BLACK);
        DrawPoly(shipPosition,3,40.0f,shiprotation-90.0f,RAYWHITE);
        for(int i=0;i<MAX_BULLETS;i++)
        {
            if(bullet[i].active)
            {
                DrawCircleV(bullet[i].position,5.0f,RED);
            }
        }
       
        
        EndDrawing();
    }

    CloseWindow();

    return 0;
}
