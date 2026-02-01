// physicsEffects: applies per-frame accelerations (gravity, friction) to registered Entities.
/**
 * @brief The physicsEffects class applies world forces (gravity), friction and bounce-handling.
 * Notes:
 * - All velocities are stored in pixels/s and updated per-frame using dt = GetFrameTime().
 * - This class holds non-owning Entity pointers.
 */
#ifndef physicsEffects_h
#define physicsEffects_h
#include "Entity.h"
#include <vector>

class physicsEffects {
    private:
    std::vector<Entity*> entity_ptr;
    public:
    physicsEffects() = default;

    /** Apply gravity, friction and simple bounce resolution once per frame. */
    void applyGravity();

    /** Register a non-owning entity pointer for physics updates (duplicates ignored). */
    void addToEntityList(Entity *entity);

    /** Unregister an entity pointer. */
    void removeFromEntityList(Entity *entity);
};
#endif // physicsEffects_h