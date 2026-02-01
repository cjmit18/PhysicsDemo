#include <cmath>
#include "physicsEffects.h"
#include "Entity.h"
#include "raylib.h"
#include "config.h"
#include <algorithm>


void physicsEffects::applyGravity(){
    double dt = GetFrameTime();
    for (Entity *entity : entity_ptr) {
        if (!entity) continue;
        // If entity is on the ground and NOT bouncy, keep it clamped and skip gravity.
        if (entity->getOnGround() && !entity->isEntityBouncy()) {
            entity->set_vy(0.0);
            entity->set_y(HEIGHT - entity->get_radius());
            entity->set_x(entity->get_x() + entity->get_vx() * dt);
            continue;
        }
        // Gravity is an acceleration (pixels/s^2). Apply per-frame velocity change.
        entity->addToVy(GRAVITY * dt);
        if (entity->get_vy() > MAX_FALL_SPEED)
            entity->set_vy(MAX_FALL_SPEED);
            
        entity->set_y(entity->get_y() + entity->get_vy() * dt);
        entity->set_x(entity->get_x() + entity->get_vx() * dt);

        // Ground collision handling: clamp to floor and resolve vertical velocity.
        if (entity->get_y() + entity->get_radius() >= HEIGHT) {
            entity->set_y(HEIGHT - entity->get_radius());
            // Use weight (mass) to influence bounce/fractional response.
            double mass = std::max(1.0, entity->getWeight());
            // massBounceFactor reduces rebound for heavier objects (tweak `k` to taste)
            const double k = 0.02; // tuning constant
            double massBounceFactor = 1.0 / (1.0 + (mass - 1.0) * k);

            if (entity->isEntityBouncy()) {
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
        if (entity->isEntityBouncy()) {
            if (entity->get_x() + entity->get_radius() >= WIDTH || entity->get_x() - entity->get_radius() <= 0) {
                entity->set_vx(-entity->get_vx() * BOUNCE); // simple horizontal bounce on side walls
            }
            // skip non-bouncy friction logic for bouncy entities
            continue;
        }
        // Apply friction to horizontal velocity; heavier objects decay slower.
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
    entity_ptr.push_back(entity);
}
void physicsEffects::removeFromEntityList(Entity *entity){
    entity_ptr.erase(std::remove(entity_ptr.begin(), entity_ptr.end(), entity), entity_ptr.end());
}
