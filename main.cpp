#include "raylib.h"
#include "Entity.h"
#include "physicsEffects.h"
#include "inputManager.h"
#include "windowInteractionss.h"
#include "config.h"
#include <ctime>
#include <cmath>
#include <vector>

double x = WIDTH/2;
double y = HEIGHT/2;

// Pre-size the players vector to avoid out-of-bounds access on startup
std::vector<Entity*> players(MAX_ENTITIES);
PhysicsEffects physics;
inputManager inputMgr;
windowInteractions windowInt;

void initializePlayers(){
  // Create a pool of entities with randomized starting positions and small initial horizontal velocity.
  // Names are generated for debug; color set to RED initially.
  SetRandomSeed(time(NULL));
  for (int i{0}; i < MAX_ENTITIES; i++){
    players[i] = new Entity(("player "+ std::to_string(i+1)).c_str(),
                            GetRandomValue(50, WIDTH-50),
                            GetRandomValue(50, HEIGHT-50),
                            0, 5, RED);
    //players[i]->set_vx(GetRandomValue(-20,20));
    //players[i]->set_vy(GetRandomValue(-20,20));
    physics.addToEntityList(players[i]);
    inputMgr.addToEntityList(players[i]);
    windowInt.addToEntityList(players[i]);
  }
}

void drawPlayers(){
  // Draw each active entity as a circle using the entity's stored color and radius.
  for (int i{0}; i < MAX_ENTITIES; i++){
    if (!players[i]) continue;
    DrawCircle(players[i]->get_x(), players[i]->get_y(), players[i]->get_radius(), players[i]->get_color());
  }
}

static void resolveCollision(Entity *a, Entity *b) {
  // Resolve interpenetration by moving objects proportionally to their "mass" (radius).
  // Then compute an impulse along the collision normal using a restitution coefficient.
  // This keeps objects from overlapping and gives a simple bounce response.
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

  a->addToVx(jx / ma); // was: a->set_vx(a->get_vx() + jx / ma);
  a->addToVy(jy / ma); // was: a->set_vy(a->get_vy() + jy / ma);
  b->addToVx(-jx / mb); // was: b->set_vx(b->get_vx() - jx / mb);
  b->addToVy(-jy / mb); // was: b->set_vy(b->get_vy() - jy / mb);
}

void updatePlayerPositions(){
  // Per-frame update:
  // 1) reset collision flags/colors
  // 2) apply input, physics and bounds per entity
  // 3) detect pairwise circle collisions and resolve them
  // Entities flagged for deletion are cleaned up here.
  // Reset colliding state and color each frame
  // Process inputs and physics once per frame (not per-entity)
  inputMgr.processInputs();
  physics.applyGravity();
  windowInt.checkAllBounds();

  for (int i{0}; i < MAX_ENTITIES; i++){
    if (!players[i]) {
      continue;
    }
    if (players[i]->isMarkedForDeletion()) {
        physics.removeFromEntityList(players[i]);
        inputMgr.removeFromEntityList(players[i]);
        windowInt.removeFromEntityList(players[i]);
        delete players[i];
        players[i] = nullptr;
        continue;
    }
    Vector2 center1 = {static_cast<float>(players[i]->get_x()), static_cast<float>(players[i]->get_y())};
    for (int j{i+1}; j < MAX_ENTITIES; j++){
      if (!players[j]) continue;
      Vector2 center2 = {static_cast<float>(players[j]->get_x()), static_cast<float>(players[j]->get_y())};
      if (CheckCollisionCircles(center1, static_cast<float>(players[i]->get_radius()), center2, static_cast<float>(players[j]->get_radius()))) {
        players[i]->setColliding(true);
        players[j]->setColliding(true);
        resolveCollision(players[i], players[j]);
      }
    }
  }
}

int main() {
  // Initialize window, spawn entities and run simulation loop at fixed target FPS.
  InitWindow(WIDTH, HEIGHT, "Basic Physics Simulation");
  initializePlayers();
  players[0]->setCanMove(true);
  SetTargetFPS(60);
  while (!WindowShouldClose()) {
    updatePlayerPositions();
    BeginDrawing();
    DrawFPS(820,0);
    if (players[0]) {
      players[0]->showInfo();
    }
    ClearBackground(RAYWHITE);
    drawPlayers(); 
    EndDrawing();
  }
  CloseWindow();
  return 0;
}