#include <cmath>
#include "PhysicsEffects.h"
#include "Entity.h"
#include "raylib.h"
#include "config.h"
#include <algorithm>


void PhysicsEffects::applyGravity(){
    float dt = GetFrameTime();
    for (Entity *entity : entity_ptr) {
        if (!entity) continue;
        if (std::abs(entity->get_vy()) < 1e-6 && std::abs((entity->get_y() + entity->get_radius()) - HEIGHT) < 1e-6) {
            entity->set_x(entity->get_x() + entity->get_vx() * dt * SPEED_MULT);
            continue;
        }
        entity->addToVy(GRAVITY * dt * SPEED_MULT); // changed from set_vy(get_vy() + ...)
        if (entity->get_vy() > MAX_FALL_SPEED) entity->set_vy(MAX_FALL_SPEED);
        entity->set_y(entity->get_y() + entity->get_vy() * dt * SPEED_MULT);
        entity->set_x(entity->get_x() + entity->get_vx() * dt * SPEED_MULT);

        if (entity->get_y() + entity->get_radius() >= HEIGHT) {
            entity->set_y(HEIGHT - entity->get_radius());
            double preVy = entity->get_vy();
            double targetVy = -preVy * BOUNCE; // desired post-bounce upward velocity (negative)
            if (targetVy < -MAX_FLY_SPEED) {
                targetVy = -MAX_FLY_SPEED; // clamp upward speed magnitude
            }
            entity->set_vy(targetVy); // assign clamped bounce velocity
            if (std::abs(entity->get_vy()) < 0.3) {
                entity->set_vy(0.0);
            }
        }
        if (entity->get_x() + entity->get_radius() >= WIDTH || entity->get_x() - entity->get_radius() <= 0) {
            entity->set_vx(-entity->get_vx() * BOUNCE); // simple horizontal bounce on side walls
        }
        // Apply friction to horizontal velocity
        double decay = std::pow(FRICTION, dt * SPEED_MULT);
        double newVx = entity->get_vx() * decay;
         if (std::abs(newVx) < 0.01) newVx = 0.0;
             entity->set_vx(newVx);
    }
}

void PhysicsEffects::addToEntityList(Entity *entity){
    entity_ptr.push_back(entity);
}
void PhysicsEffects::removeFromEntityList(Entity *entity){
    entity_ptr.erase(std::remove(entity_ptr.begin(), entity_ptr.end(), entity), entity_ptr.end());
}
