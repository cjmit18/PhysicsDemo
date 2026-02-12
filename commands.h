#include "raylib.h"
#include "Entity.h"
#include "physicsEffects.h"
#include "inputManager.h"
#include "windowInteractions.h"
#include "config.h"
#include <ctime>

// Globals are defined in commands.cpp to avoid multiple-definition linker errors.
extern std::vector<Entity*> players;
extern double x;
extern double y;
extern physicsEffects physics;
extern inputManager inputMgr;
extern windowInteractions windowInt;

// Function prototypes implemented in commands.cpp
void initializePlayers();
void SpawnEntity(double x, double y, double radius, double weight, Color color, int nEnts);
void drawPlayers();

