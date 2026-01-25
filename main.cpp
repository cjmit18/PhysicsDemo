#include "raylib.h"
#include "Entity.h"

double x = 400;
double y = 400;
double rad = 100;
Entity *players[5];
Entity player = Entity("Player1", x, y, rad);
void checkInput(Entity *ent) {
    ent->checkInput();
}
void checkBounds(Entity *ent) {
  ent->checkBounds();
}
int main() {
  const int screenWidth = 900;
  const int screenHeight = 600;
  InitWindow(screenWidth, screenHeight, "raylib basic window");
  SetTargetFPS(60);
  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    DrawCircle(player.get_x(), player.get_y(), player.get_radius(), RED);
    checkInput(&player);
    checkBounds(&player);
    EndDrawing();

  }
  CloseWindow();
  return 0;
}