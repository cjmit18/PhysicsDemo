// physicsEffects implementation: gravity integration, bounce handling, and friction damping.
// All updates use dt = GetFrameTime() and treat GRAVITY as pixels/s^2.

#include <cmath>
#include "physicsEffects.h"
#include "Entity.h"
#include "raylib.h"
#include "config.h"
#include <algorithm>


void physicsEffects::applyGravity(){
    double dt = GetFrameTime();
    int height = GetScreenHeight();

    for (Entity *entity : entity_ptr) {
        if (!entity) continue;
        // If entity is on the ground and NOT bouncy, keep it clamped and skip gravity.
        if (entity->getOnGround() && !entity->getEntityBouncy()) {
            entity->set_vy(0.0);
            entity->set_y(height - entity->get_radius());
            entity->set_x(entity->get_x() + entity->get_vx() * dt);
            continue;
        }
        // Gravity is an acceleration (pixels/s^2). Apply per-frame velocity change.
        entity->addToVy(GRAVITY * dt);
        if (entity->get_vy() > MAX_FALL_SPEED)
            entity->set_vy(MAX_FALL_SPEED);

        // Integrate positions using velocity * dt (consistent units)
        entity->set_y(entity->get_y() + entity->get_vy() * dt);
        entity->set_x(entity->get_x() + entity->get_vx() * dt);

        // Ground collision handling: clamp to floor and resolve vertical velocity.
        if (entity->get_y() + entity->get_radius() >= height) {
            entity->set_y(height - entity->get_radius());
            // Use weight (mass) to influence bounce response in a stable way.
            double mass = std::max(1.0, entity->getWeight());
            // massBounceFactor reduces rebound for heavier objects (tunable constant)
            const double k = 0.02; // tuning constant
            double massBounceFactor = 1.0 / (1.0 + (mass - 1.0) * k);

            if (entity->getEntityBouncy()) {
                double preVy = entity->get_vy();
                double targetVy = -preVy * BOUNCE * massBounceFactor; // mass-scaled rebound
                if (targetVy < -MAX_FLY_SPEED) {
                    targetVy = -MAX_FLY_SPEED; // clamp upward speed magnitude
                }
                entity->set_vy(targetVy); // assign clamped bounce velocity
                // Only mark bouncy entity as on-ground if bounce is effectively finished
                if (std::abs(entity->get_vy()) < 0.3) {
                    entity->set_vy(0.0);
                    entity->setOnGround(true);
                }
            } 
            else {
                // Non-bouncy: stop downward movement and mark on-ground
                if (entity->get_vy() > 0.0) {
                    entity->set_vy(0.0);
                }
                entity->setOnGround(true);
            }
        }

        // Side-wall bounce for bouncy entities
        if (entity->getEntityBouncy()) {
             int width = GetScreenWidth();
            if (entity->get_x() + entity->get_radius() >= width || entity->get_x() - entity->get_radius() <= 0) {
                entity->set_vx(-entity->get_vx() * BOUNCE); // simple horizontal bounce on side walls
            }
            // skip non-bouncy friction logic for bouncy entities
            continue;
        }
        // Apply friction to horizontal velocity; heavier objects decay slower.
        // Friction constant is per-second retention; per-frame decay = pow(FRICTION, dt / mass)
        double mass_for_friction = std::max(1.0, entity->getWeight());
        double decay = std::pow(FRICTION, dt * (1.0 / mass_for_friction));
        double newVx = entity->get_vx() * decay;
         if (std::abs(newVx) < 0.01) {
            newVx = 0.0;
        }
        entity->set_vx(newVx);
    }
}

void physicsEffects::addToEntityList(Entity *entity){
    for (int i = 0; i < entity_ptr.size(); i++) {
        if (entity_ptr[i] == entity) {
            return; // already in list
        }
        if (entity_ptr[i] == nullptr) {
            entity_ptr[i] = entity;
            return; // added in empty slot
        }
    }
    // If we reach here, no empty slot was found; add to the end
    entity_ptr.push_back(entity);
}
void physicsEffects::removeFromEntityList(Entity *entity){
    for (size_t i = 0; i < entity_ptr.size(); i++) {
        if (entity_ptr[i] == entity) {
            entity_ptr[i] = nullptr; // mark slot as empty
            return;
        }
        if (entity_ptr[i] == nullptr) {
            continue; // skip empty slots
        }
    }
    entity_ptr.erase(std::remove(entity_ptr.begin(), entity_ptr.end(), entity), entity_ptr.end());
}
