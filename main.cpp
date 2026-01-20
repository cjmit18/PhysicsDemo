#include "raylib.h"
double x = 400;
double y = 400;
double rad = 100;
int main() {
  const int screenWidth = 800;
  const int screenHeight = 600;
  InitWindow(screenWidth, screenHeight, "raylib basic window");
  SetTargetFPS(60);
  while (!WindowShouldClose()) {
   SetTargetFPS(144);
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawCircle(x, y, rad, RED);

    if (IsKeyDown(KEY_RIGHT)){
      x += 1;
    }
    if (IsKeyDown(KEY_LEFT)){
      x-= 1;
    }

    if (IsKeyDown(KEY_UP)){
      y -= 1;
    }
    if (IsKeyDown(KEY_DOWN)){
      y+=1;
    }
    if (IsKeyPressed(KEY_MINUS)){
      rad -= 10;
    }
    if (IsKeyPressed(KEY_EQUAL)){
      rad += 10;
    }
    
    EndDrawing();
  }
  CloseWindow();
  return 0;
}