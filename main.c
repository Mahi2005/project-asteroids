#include "raylib.h"


int main(void) {
  InitWindow(1200,600,"Game");
  SetTargetFPS(60);
  while(!WindowShouldClose())
    {
      BeginDrawing();
      ClearBackground(RED);
      DrawText("Hello, World!", 0, 0, 20, WHITE);
      EndDrawing();
    }
  CloseWindow();
}
