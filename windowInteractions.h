// windowInteractions: helper to keep Entities within window bounds and set boundary flags.
/**
 * @brief Manage per-window interactions for Entities (bounds clamping, side/ceiling/floor flags).
 *
 * This class holds raw pointers (non-owning) and provides:
 * - addToEntityList / removeFromEntityList: register/unregister entities
 * - checkAllBounds: clamp positions to current screen size and set state flags
 */
#ifndef windowInteractions_h
#define windowInteractions_h
#include "Entity.h"
#include <vector>

class windowInteractions {
    private:
    std::vector<Entity*> entity_ptr;
    public:
    windowInteractions() = default;

    /**
     * @brief Add a non-owning pointer to be checked each frame.
     * Duplicate pointers are ignored; first available nullptr slot is reused.
     */
    void addToEntityList(Entity* e);

    /**
     * @brief Remove a pointer from the list (all occurrences removed).
     */
    void removeFromEntityList(Entity* e);

    /**
     * @brief Clamp each registered entity to the current screen and set boundary flags.
     * Called once per frame by the main loop.
     */
    void checkAllBounds();

};
#endif //