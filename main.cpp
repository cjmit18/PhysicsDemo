#include "raylib.h"
#include "Entity.h"
#include <ctime>
#include <cmath>
#define WIDTH 900
#define HEIGHT 600
#define GRAVITY 1.0
#define MAX_PLAYERS 1

double x = WIDTH/2;
double y = HEIGHT/2;

Entity *players[MAX_PLAYERS];

void initializePlayers(){
  SetRandomSeed(time(NULL));
  for (int i{0}; i < MAX_PLAYERS; i++){
    players[i] = new Entity((std::string("Player") + std::to_string(i+1)).c_str(), GetRandomValue(50, WIDTH-50), GetRandomValue(50, HEIGHT-50), 0, GetRandomValue(20, 50), RED);
  }
}

void drawPlayers(){
  for (int i{0}; i < MAX_PLAYERS; i++){
    DrawCircle(players[i]->get_x(), players[i]->get_y(), players[i]->get_radius(), players[i]->get_color());
  }
}

// Resolve overlap and apply simple impulse between two circles
static void resolveCollision(Entity *a, Entity *b) {
  double dx = a->get_x() - b->get_x();
  double dy = a->get_y() - b->get_y();
  double dist = std::sqrt(dx*dx + dy*dy);
  if (dist == 0.0) { dx = 1.0; dist = 1.0; } // avoid div by zero

  double overlap = (a->get_radius() + b->get_radius()) - dist;
  if (overlap <= 0.0) return;

  // Normal vector
  double nx = dx / dist;
  double ny = dy / dist;

  // Use radius as proxy for mass
  double ma = std::max(1.0, a->get_radius());
  double mb = std::max(1.0, b->get_radius());
  double total = ma + mb;

  // Separate proportional to mass
  a->set_x(a->get_x() + nx * (overlap * (mb / total)));
  a->set_y(a->get_y() + ny * (overlap * (mb / total)));
  b->set_x(b->get_x() - nx * (overlap * (ma / total)));
  b->set_y(b->get_y() - ny * (overlap * (ma / total)));

  // Relative velocity
  double rvx = a->get_vx() - b->get_vx();
  double rvy = a->get_vy() - b->get_vy();
  double velAlongNormal = rvx * nx + rvy * ny;

  // If they're separating, do not apply impulse
  if (velAlongNormal > 0.0) return;

  // Restitution (bounciness)
  double e = 0.6;

  // Impulse scalar
  double j = -(1.0 + e) * velAlongNormal;
  j /= (1.0/ma + 1.0/mb);

  double jx = j * nx;
  double jy = j * ny;

  a->set_vx(a->get_vx() + jx / ma);
  a->set_vy(a->get_vy() + jy / ma);
  b->set_vx(b->get_vx() - jx / mb);
  b->set_vy(b->get_vy() - jy / mb);
}

void updatePlayerPositions(){
  // Reset colliding state and color each frame
  for (int i = 0; i < MAX_PLAYERS; ++i) {
    players[i]->setColliding(false);
    players[i]->set_color(RED); // reset to default (matches initializePlayers)
  }

  for (int i{0}; i < MAX_PLAYERS; i++){
    players[i]->objectMovement(WIDTH, HEIGHT, GRAVITY);
    Vector2 center1 = {static_cast<float>(players[i]->get_x()), static_cast<float>(players[i]->get_y())};
    for (int j{i+1}; j < MAX_PLAYERS; j++){
      Vector2 center2 = {static_cast<float>(players[j]->get_x()), static_cast<float>(players[j]->get_y())};
      if (CheckCollisionCircles(center1, static_cast<float>(players[i]->get_radius()), center2, static_cast<float>(players[j]->get_radius()))) {
        players[i]->setColliding(true);
        players[j]->setColliding(true);
        players[i]->set_color(BLUE);
        players[j]->set_color(BLUE);
        resolveCollision(players[i], players[j]);
      }
    }
  }
}

int main() {
  initializePlayers();
  InitWindow(WIDTH, HEIGHT, "MAIN WINDOW");
  SetTargetFPS(60);
  while (!WindowShouldClose()) {
    // update first so draw uses the current collision colors/positions
    updatePlayerPositions();
    BeginDrawing();
    ClearBackground(RAYWHITE);
    drawPlayers();
    EndDrawing();
  }
  CloseWindow();
  return 0;
}