#include "raylib.h"
#include "Entity.h"
#include <ctime>
#define WIDTH 900
#define HEIGHT 600
#define GRAVITY 1.0

double x = WIDTH/2;
double y = HEIGHT/2;

Entity *players[5];
void initializePlayers(){
  SetRandomSeed(time(NULL));
  for (int i{0}; i < 5; i++){
    players[i] = new Entity("Player" + std::to_string(i+1), GetRandomValue(50, WIDTH-50), GetRandomValue(50, HEIGHT-50), 20.0);
  }
}
int main() {
  initializePlayers();
  InitWindow(WIDTH, HEIGHT, "MAIN WINDOW");
  SetTargetFPS(60);
  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(RAYWHITE);
    for (int i{0}; i < 5; i++){
      DrawCircle(players[i]->get_x(), players[i]->get_y(), players[i]->get_radius(), MAROON);
    }
    for (int i{0}; i < 5; i++){
      players[i]->objectMovement(WIDTH, HEIGHT, GRAVITY);
    }
  EndDrawing();
  }
  CloseWindow();
return 0;
  }