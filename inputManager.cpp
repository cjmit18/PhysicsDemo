#include "Entity.h"
#include "raylib.h"
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

        // Handle input as forces/accelerations (units: pixels/s^2) and apply using dt
        // Horizontal movement: treat WALK_SPEED as acceleration; clamp to MAX_WALK_SPEED
        if (IsKeyDown(KEY_A) && IsKeyDown(KEY_D)) {
            entity->set_vx(0.0);
        }
        else if (IsKeyDown(KEY_D)) {
            double mass = entity->getWeight(); if (mass <= 0.0) mass = 1.0;
            entity->addToVx((WALK_SPEED / mass) * dt);
            if (entity->get_vx() > MAX_WALK_SPEED) {
                entity->set_vx(MAX_WALK_SPEED);
            }
        }
        else if (IsKeyDown(KEY_A)) {
            double mass = entity->getWeight(); if (mass <= 0.0) mass = 1.0;
            entity->addToVx((-WALK_SPEED / mass) * dt);
            if (entity->get_vx() < -MAX_WALK_SPEED) {
                entity->set_vx(-MAX_WALK_SPEED);
            }
        }

        // Vertical movement: FLYSPEED/FALL_SPEED treated as accelerations (or forces that cancel mass)
        if (IsKeyDown(KEY_W) && IsKeyDown(KEY_S)) {
            // no vertical input; gravity handled in physicsEffects
        }
        else if (IsKeyDown(KEY_W)) {
            double mass = entity->getWeight(); if (mass <= 0.0) mass = 1.0;
            entity->addToVy((-FLYSPEED / mass) * dt);
            if (entity->get_vy() < -MAX_FLY_SPEED) {
                entity->set_vy(-MAX_FLY_SPEED);
            }
        }
        else if (IsKeyDown(KEY_S)) {
            double mass = entity->getWeight(); if (mass <= 0.0) mass = 1.0;
            entity->addToVy((FALL_SPEED / mass) * dt);
            if (entity->get_vy() > MAX_FALL_SPEED) {
                entity->set_vy(MAX_FALL_SPEED);
            }
        }
        if (IsKeyPressed(KEY_SPACE)) {
            if (entity->getOnGround()) {
                double mass = entity->getWeight(); if (mass <= 0.0) mass = 1.0;
                entity->set_vy(-FLYSPEED / mass); // instant jump impulse
                entity->setOnGround(false);
            }
        }
        // When the entity can move but no horizontal keys are pressed, apply damping
        if (entity->getCanMove() && !(IsKeyDown(KEY_D) || IsKeyDown(KEY_A))) {
            // Use same FRICTION semantics as physicsEffects: per-second retention -> per-frame decay = FRICTION^dt
            double decay = std::pow(static_cast<double>(FRICTION), static_cast<double>(dt));
            entity->set_vx(entity->get_vx() * decay);
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
        if (!(IsKeyPressed(KEY_W) && IsKeyPressed(KEY_S) && IsKeyPressed(KEY_A) && IsKeyPressed(KEY_D))) {
            entity->setStatic(true);
        } 
        else {
            entity->setStatic(false);
        }

    }

}