#ifndef PhysicsEffects_h
#define PhysicsEffects_h
#include "Entity.h"
#include <vector>

class PhysicsEffects {
    private:
    std::vector<Entity*> entity_ptr;
    int WIDTH{900};
    int HEIGHT{600};
     
    public:
    PhysicsEffects(int WIDTH, int HEIGHT);

    void applyGravity(); 
    void addToEntityList(Entity *entity);
};
#endif // PhysicsEffects_h