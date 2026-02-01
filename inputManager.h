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

    void addToEntityList(Entity *entity);
    void removeFromEntityList(Entity *entity);
    void processInputs();
};


#endif // INPUTMANAGER_H