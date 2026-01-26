#include "raylib.h"
#include "Entity.h"
#include <ctime>
#include <cmath>
#define WIDTH 900
#define HEIGHT 600
#define GRAVITY 0.50
#define MAX_PLAYERS 1
double x = WIDTH/2;
double y = HEIGHT/2;

Entity *players[MAX_PLAYERS];

void initializePlayers(){
  SetRandomSeed(time(NULL));
  for (int i{0}; i < MAX_PLAYERS; i++){
    players[i] = new Entity((std::string("Player") + std::to_string(i+1)).c_str(),
                            GetRandomValue(50, WIDTH-50),
                            GetRandomValue(50, HEIGHT-50),
                            0, 8, RED);
    // lower initial random velocity so input and collisions behave predictably
    players[i]->set_vx(GetRandomValue(-20,20));
    players[i]->set_vy(GetRandomValue(-20,20));
  }
}

void drawPlayers(){
  for (int i{0}; i < MAX_PLAYERS; i++){
    if (!players[i]) continue;
    DrawCircle(players[i]->get_x(), players[i]->get_y(), players[i]->get_radius(), players[i]->get_color());
  }
}

static void resolveCollision(Entity *a, Entity *b) {
  double dx = a->get_x() - b->get_x(); // delta x
  double dy = a->get_y() - b->get_y(); // delta y
  double dist = std::sqrt(dx*dx + dy*dy); // distance between centers
  if (dist == 0.0) { dx = 1.0; dist = 1.0; } // avoid div by zero

  double overlap = (a->get_radius() + b->get_radius()) - dist; // penetration depth
  if (overlap <= 0.0) return; 

  // Normal vector
  double nx = dx / dist; // normalized delta x
  double ny = dy / dist; // normalized delta y

  // Use radius as proxy for mass
  double ma = std::max(1.0, a->get_radius()); // mass of a
  double mb = std::max(1.0, b->get_radius()); // mass of b
  double total = ma + mb;

  // Separate proportional to mass
  a->set_x(a->get_x() + nx * (overlap * (mb / total))); // move a out
  a->set_y(a->get_y() + ny * (overlap * (mb / total))); // move a out
  b->set_x(b->get_x() - nx * (overlap * (ma / total))); // move b out
  b->set_y(b->get_y() - ny * (overlap * (ma / total))); // move b out
 
  // Relative velocity
  double rvx = a->get_vx() - b->get_vx(); // delta vx
  double rvy = a->get_vy() - b->get_vy(); // delta vy
  double velAlongNormal = rvx * nx + rvy * ny; // velocity along normal

  // If they're separating, do not apply impulse
  if (velAlongNormal > 0.0) return;

  // Restitution (bounciness)
  double e = 0.6;

  // Impulse scalar
  double j = -(1.0 + e) * velAlongNormal; 
  j /= (1.0/ma + 1.0/mb); 

  double jx = j * nx; // impulse x
  double jy = j * ny; // impulse y

  a->set_vx(a->get_vx() + jx / ma); // update velocity of a
  a->set_vy(a->get_vy() + jy / ma); // update velocity of a
  b->set_vx(b->get_vx() - jx / mb); // update velocity of b
  b->set_vy(b->get_vy() - jy / mb); // update velocity of b
}

void updatePlayerPositions(){
  // Reset colliding state and color each frame
  for (int i = 0; i < MAX_PLAYERS; ++i) {
    if (!players[i]) continue;
    players[i]->setColliding(false);
    players[i]->set_color(RED); // reset to default (matches initializePlayers)
  }

  for (int i{0}; i < MAX_PLAYERS; i++){
    if (!players[i]) continue;
    players[i]->objectMovement(WIDTH, HEIGHT, GRAVITY);
    if (players[i]->isMarkedForDeletion()) {
      delete players[i];
      players[i] = nullptr;
      continue;
    }
    Vector2 center1 = {static_cast<float>(players[i]->get_x()), static_cast<float>(players[i]->get_y())};
    for (int j{i+1}; j < MAX_PLAYERS; j++){
      if (!players[j]) continue;
      Vector2 center2 = {static_cast<float>(players[j]->get_x()), static_cast<float>(players[j]->get_y())};
      if (CheckCollisionCircles(center1, static_cast<float>(players[i]->get_radius()), center2, static_cast<float>(players[j]->get_radius()))) {
        players[i]->setColliding(true);
        players[j]->setColliding(true);
        resolveCollision(players[i], players[j]);
      }
    }
  }
  // drawPlayers();  // <- removed: drawing must happen between BeginDrawing() and EndDrawing()
}

int main() {
  initializePlayers(); // initialize after window if you rely on raylib for random/colors
  InitWindow(WIDTH, HEIGHT, "Basic Physics Simulation");
  SetTargetFPS(60);
  while (!WindowShouldClose()) {
    updatePlayerPositions();
    BeginDrawing();
    DrawFPS(820,0);
    ClearBackground(RAYWHITE);
    drawPlayers(); // draw inside drawing block
    EndDrawing();
  }
  CloseWindow();
  return 0;
}