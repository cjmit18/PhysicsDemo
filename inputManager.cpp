#include "Entity.h"
#include "raylib.h"
#include "PhysicsEffects.h"
#include "inputManager.h"
#include "config.h"
#include <vector>
#include <cmath> // added for std::abs
#include <algorithm>

void inputManager::addToEntityList(Entity *entity){
    entity_ptr.push_back(entity);
}
void inputManager::removeFromEntityList(Entity *entity) {
    entity_ptr.erase(std::remove(entity_ptr.begin(), entity_ptr.end(), entity), entity_ptr.end());
}
void inputManager::processInputs(){
    float dt = GetFrameTime();
    for (Entity *entity : entity_ptr) {
        if (!entity) continue;
        if (!entity->getCanMove()) continue;
        // Horizontal movement
        // Set instantaneous horizontal velocity (don't multiply by dt)
        if (IsKeyDown(KEY_D)) {
            entity->set_vx(WALK_SPEED * SPEED_MULT);
            if (entity->get_vx() > MAX_WALK_SPEED) {
                entity->set_vx(MAX_WALK_SPEED);
            }
        } 
        else if (IsKeyDown(KEY_A)) {
            entity->set_vx(-WALK_SPEED * SPEED_MULT);
            if (entity->get_vx() < -MAX_WALK_SPEED) {
                entity->set_vx(-MAX_WALK_SPEED);
            }
        }
        // Vertical movement
        if (IsKeyDown(KEY_W)) {
            entity->addToVy(-FLYSPEED * dt * SPEED_MULT);
            if (entity->get_vy() < -MAX_FLY_SPEED) {
                entity->set_vy(-MAX_FLY_SPEED);
            }
        }
        if (IsKeyDown(KEY_S)) {
            entity->addToVy(FALL_SPEED * dt * SPEED_MULT);
            if (entity->get_vy() > MAX_FALL_SPEED) {
                entity->set_vy(MAX_FALL_SPEED);
            }
        }
        // When the entity can move but no horizontal keys are pressed, apply damping
        if (entity->getCanMove() && !(IsKeyDown(KEY_D) || IsKeyDown(KEY_A))) {
            entity->set_vx(entity->get_vx() * 0.8);
            if (std::abs(entity->get_vx()) < 0.05){
                entity->set_vx(0.0);
            }
        }
        if (IsKeyDown(KEY_EQUAL)) {
            entity->set_radius(entity->get_radius() + 1.0);
        }
        if (IsKeyDown(KEY_MINUS)) {
            entity->set_radius(entity->get_radius() - 1.0);
        }
        if (IsKeyDown(KEY_DELETE)) {
            entity->markedForDeletionStatus(true);
            }
    }
}