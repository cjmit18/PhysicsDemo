#ifndef PhysicsEffects_h
#define PhysicsEffects_h
#include "Entity.h"
#include <vector>

class PhysicsEffects {
    private:
    std::vector<Entity*> entity_ptr;
    public:
    PhysicsEffects() = default;

    void applyGravity();
    void addToEntityList(Entity *entity);
    void removeFromEntityList(Entity *entity);
};
#endif // PhysicsEffects_h