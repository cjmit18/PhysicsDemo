#ifndef physicsEffects_h
#define physicsEffects_h
#include "Entity.h"
#include <vector>

class physicsEffects {
    private:
    std::vector<Entity*> entity_ptr;
    public:
    physicsEffects() = default;

    void applyGravity();
    void addToEntityList(Entity *entity);
    void removeFromEntityList(Entity *entity);
};
#endif // physicsEffects_h