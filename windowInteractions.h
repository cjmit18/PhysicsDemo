#ifndef windowInteractions_h
#define windowInteractions_h
#include "Entity.h"
#include <vector>

class windowInteractions {
    private:
    std::vector<Entity*> entity_ptr;
    public:
    windowInteractions() = default;
    void addToEntityList(Entity* e);
    void removeFromEntityList(Entity* e);
    void checkAllBounds();

};
#endif // 