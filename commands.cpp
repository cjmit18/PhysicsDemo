#include "commands.h"
#include "rlgl.h"
#include <cstdlib>

// Define globals (single definition)
std::vector<Entity*> players(MAX_ENTITIES, nullptr);
double x = 0.0;
double y = 0.0;
physicsEffects physics;
inputManager inputMgr;
windowInteractions windowInt;

void initializePlayers(){
  // Create a pool of entities with randomized starting positions and small initial horizontal velocity.
  // Use current screen center at initialization time to avoid calling raylib before InitWindow.
  SetRandomSeed(time(NULL));
  double centerX = static_cast<double>(GetScreenWidth()) / 2.0;
  double centerY = static_cast<double>(GetScreenHeight()) / 2.0;
  for (int i{0}; i < INITIAL_ENTITIES; i++){
    Entity* player = new Entity("player "+ std::to_string(i+1),
                                  GetRandomValue(0,centerX*2),
                                  GetRandomValue(0,centerY*2),
                                  0, GetRandomValue(1,5), GetRandomValue(1,100), RED);
    if (player == nullptr) continue;
    players[i] = player;
    players[i]->set_vx(GetRandomValue(-20,20));
    players[i]->set_vy(GetRandomValue(-20,20));
    physics.addToEntityList(players[i]);
    inputMgr.addToEntityList(players[i]);
    windowInt.addToEntityList(players[i]);
  }
}

void SpawnEntity(double x, double y, double radius, double weight, Color color, int nEnts){
  // Spawn up to nEnts into available pre-sized slots without resize/push_back
  int spawned = 0;
  for (int i = 0; i < MAX_ENTITIES && spawned < nEnts; ++i) {
    if (players[i] != nullptr) continue;
    Entity* newEntity = new Entity("player " + std::to_string(i+1), x, y, 0, radius, weight, color);
    if (!newEntity) break;
    players[i] = newEntity;
    physics.addToEntityList(players[i]);
    inputMgr.addToEntityList(players[i]);
    windowInt.addToEntityList(players[i]);
    ++spawned;
  }
}

void drawPlayers(){
  // Ensure there's room in rlgl batch
  rlCheckRenderBatchLimit(MAX_ENTITIES * 6);

    // Debug: draw using raylib's DrawCircle to verify entities are visible
    for (int i = 0; i < MAX_ENTITIES; ++i) {
      if (!players[i]) continue;
      DrawCircle(static_cast<int>(players[i]->get_x()), static_cast<int>(players[i]->get_y()),
                 static_cast<float>(players[i]->get_radius()), players[i]->get_color());
    }
}
