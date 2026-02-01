// Main loop and collision detection/resolution for physics demo.
// Key notes:
//  - resolveCollision uses weight (or radius) as mass, clamps per-step positional correction, and avoids divide-by-zero by using deterministic jitter.
//  - DetectCollison iterates valid pointers only and resets flags before collision pass.

#include "raylib.h"
#include "Entity.h"
#include "physicsEffects.h"
#include "inputManager.h"
#include "windowInteractions.h"
#include "config.h"
#include <ctime>
#include <cmath>
#include <vector>

double x = GetScreenWidth()/2;
double y = GetScreenHeight()/2;
int width = 2560;
int height = 13f00;

// Pre-size the players vector to avoid out-of-bounds access on startup
std::vector<Entity*> players(MAX_ENTITIES);
physicsEffects physics;
inputManager inputMgr;
windowInteractions windowInt;

void initializePlayers(){
  // Create a pool of entities with randomized starting positions and small initial horizontal velocity.
  // Names are generated for debug; color set to RED initially.
  SetRandomSeed(time(NULL));
  for (int i{0}; i < MAX_ENTITIES; i++){
    Entity* player = new Entity("player "+ std::to_string(i+1),
                            x + GetRandomValue(0,2560),
                            y + GetRandomValue(0,2000),
                            0, GetRandomValue(1,5), GetRandomValue(1,100), RED);
    if (player == nullptr) {
      continue;
    }
    if (players[i] == player) {
      continue; // already assigned
    }
    players[i] = player;
    players[i]->set_vx(GetRandomValue(-20,20));
    players[i]->set_vy(GetRandomValue(-20,20));
    physics.addToEntityList(players[i]);
    inputMgr.addToEntityList(players[i]);
    windowInt.addToEntityList(players[i]);
  }
}
void SpawnEntity(double x, double y, double radius, double weight, Color color, int nEnts){
  // Spawn a new entity at the specified position with given properties.
  for (int i{0}; i < nEnts; i++){
    if (players[i] != nullptr) {
      continue;
    }
    Entity* newEntity = new Entity("player " + std::to_string(i+1), x, y, 0, radius, weight, color);
    if (newEntity == nullptr) {
      return; // allocation failed
    }
    players[i] = newEntity;
    physics.addToEntityList(players[i]);
    inputMgr.addToEntityList(players[i]);
    windowInt.addToEntityList(players[i]);
    return; // spawned successfully
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
  // Safety: handle zero-distance case by using relative velocity or deterministic jitter to avoid NaNs.
  double dx = a->get_x() - b->get_x(); // delta x
  double dy = a->get_y() - b->get_y(); // delta y
  double dist = std::sqrt(dx*dx + dy*dy); // distance between centers

  // penetration depth
  double overlap = (a->get_radius() + b->get_radius()) - dist;
  if (overlap <= 0.0) return;

  // Normal (safe): handle degenerate zero-distance case with an epsilon
  const double eps = 1e-8;
  double nx = 0.0, ny = 0.0;
  if (dist > eps) {
    nx = dx / dist;
    ny = dy / dist;
  } else {
    // Fallback 1: use relative velocity direction (push opposite to approach)
    double rvx = a->get_vx() - b->get_vx();
    double rvy = a->get_vy() - b->get_vy();
    double rvLen = std::sqrt(rvx*rvx + rvy*rvy);
    if (rvLen > eps) {
      nx = -rvx / rvLen;
      ny = -rvy / rvLen;
    } else {
      // Fallback 2: deterministic jitter based on pointer addresses to avoid NaNs
      size_t ha = reinterpret_cast<size_t>(a);
      size_t hb = reinterpret_cast<size_t>(b);
      double seed = double((ha ^ (hb << 1)) & 0xFFFF) / double(0xFFFF);
      double angle = seed * 2.0 * PI;
      nx = std::cos(angle);
      ny = std::sin(angle);
    }
    // avoid exact-zero normal
    double nlen = std::sqrt(nx*nx + ny*ny);
    if (nlen <= eps) { nx = 1.0; ny = 0.0; nlen = 1.0; }
    nx /= nlen; ny /= nlen;
    // set a small nonzero dist so overlap computation remains sensible downstream
    dist = eps;
  }

  // Use `weight` as mass if available; fall back to radius as proxy.
  double ma_raw = a->getWeight() > 0.0 ? a->getWeight() : a->get_radius();
  double mb_raw = b->getWeight() > 0.0 ? b->getWeight() : b->get_radius();
  double ma = std::max(1.0, ma_raw); // mass of a
  double mb = std::max(1.0, mb_raw); // mass of b
  double total = ma + mb;

  // Positional correction: respect static objects and clamp per-step correction
  double moveA = 0.0, moveB = 0.0;
  if (a->getEntityStatic() && b->getEntityStatic()) {
    // both static: do not move, only resolve velocities (if desired)
    moveA = moveB = 0.0;
  } else if (a->getEntityStatic()) {
    moveA = 0.0; moveB = overlap;
  } else if (b->getEntityStatic()) {
    moveA = overlap; moveB = 0.0;
  } else {
    moveA = overlap * (mb / total);
    moveB = overlap * (ma / total);
  }
  // Clamp per-body move to at most half the overlap to avoid teleporting
  double maxPerBody = 0.5 * overlap;
  if (moveA > maxPerBody) moveA = maxPerBody;
  if (moveB > maxPerBody) moveB = maxPerBody;

  a->set_x(a->get_x() + nx * moveA);
  a->set_y(a->get_y() + ny * moveA);
  b->set_x(b->get_x() - nx * moveB);
  b->set_y(b->get_y() - ny * moveB);

  // Relative velocity (recompute if needed)
  double rvx = a->get_vx() - b->get_vx(); // delta vx
  double rvy = a->get_vy() - b->get_vy(); // delta vy
  double velAlongNormal = rvx * nx + rvy * ny; // velocity along normal

  // If they're separating, do not apply impulse
  if (velAlongNormal > 0.0) return;

  // Restitution (bounciness)
  double e = 0.6;

  // Impulse scalar with static-object safety
  double invMa = a->getEntityStatic() ? 0.0 : (1.0 / ma);
  double invMb = b->getEntityStatic() ? 0.0 : (1.0 / mb);
  double denom = (invMa + invMb);
  if (denom <= 0.0) return; // both static or invalid, skip impulse

  double j = -(1.0 + e) * velAlongNormal;
  j /= denom;

  double jx = j * nx; // impulse x
  double jy = j * ny; // impulse y

  if (!a->getEntityStatic()) { a->addToVx(jx * invMa); a->addToVy(jy * invMa); }
  if (!b->getEntityStatic()) { b->addToVx(-jx * invMb); b->addToVy(-jy * invMb); }
}

void DetectCollison(){
  // Reset per-frame flags then detect & resolve collisions between active players.
  // Only valid (non-null) pointers are considered.
  for (int i = 0; i < MAX_ENTITIES; ++i) {
    if (players[i]) players[i]->resetFlags();
  }

  // Then check collisions between valid player pointers only.
  for (int i = 0; i < MAX_ENTITIES; ++i) {
    if (!players[i]) continue;
    Vector2 center1 = {static_cast<float>(players[i]->get_x()), static_cast<float>(players[i]->get_y())};
    for (int j = i + 1; j < MAX_ENTITIES; ++j) {
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
void updatePlayerProperties(){
  // Per-frame update:
  // 1) reset collision flags/colors
  // 2) apply input, physics and bounds per entity
  // Entities flagged for deletion are cleaned up here.
  // Reset colliding state and color each frame BEFORE processing input/physics
  for (int i{0}; i < MAX_ENTITIES; i++){
    if (!players[i]) {
      continue;
    }
  }
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
  }
}

int main() {
  // Initialize window, spawn entities and run simulation loop at fixed target FPS.
  InitWindow(width, height, "Basic Physics Simulation");
  SetWindowState(FLAG_WINDOW_RESIZABLE);
  SetExitKey(KEY_NULL); // disable default ESC exit to allow in-game key handling
  initializePlayers();
  players[0]->setCanMove(true);
  players[0]->set_color(GREEN);
  players[0]->setEntityBouncy(false);
  SetTargetFPS(60);
  while (!WindowShouldClose()) {
    updatePlayerProperties();
    DetectCollison();
    BeginDrawing();
    DrawFPS(width - 100, 10);
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