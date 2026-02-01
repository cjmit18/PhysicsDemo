// inputManager: applies player inputs to registered Entities (non-owning pointers).
/**
 * @brief The inputManager reads keyboard state and converts it into velocity/impulse updates.
 * - WALK_SPEED / FLYSPEED / FALL_SPEED are treated as accelerations or impulses per second.
 * - Must be called once per frame (processInputs()).
 */
#ifndef inputManager_h
#define inputManager_h
#include "Entity.h"
#include "raylib.h"
#include <vector>

class inputManager {
    private:
     std::vector<Entity*> entity_ptr;
    public:
    inputManager() = default;

    /** Register a non-owning entity pointer (duplicates ignored). */
    void addToEntityList(Entity *entity);

    /** Unregister an entity pointer. */
    void removeFromEntityList(Entity *entity);

    /** Read keyboard state and modify velocities/flags for registered entities (one call per frame). */
    void processInputs();
};

#endif // INPUTMANAGER_H