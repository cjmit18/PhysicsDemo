#include "PhysicsEffects.h"
#include "Entity.h"
#include "raylib.h"

#define BOUNCE 0.7
#define MAX_FALL_SPEED 1.0
#define SPEED 60.0
#define GRAVITY 0.0


PhysicsEffects::PhysicsEffects(int width_value, int height_value) {
    WIDTH = width_value;
    HEIGHT = height_value;
}

void PhysicsEffects::applyGravity(){
    float dt = GetFrameTime();
    for (Entity *entity : entity_ptr) {
        if (!entity) continue;
        if (std::abs(entity->get_vy()) < 1e-6 && std::abs((entity->get_y() + entity->get_radius()) - HEIGHT) < 1e-6) {
            entity->set_x(entity->get_x() + entity->get_vx() * dt * SPEED);
            entity->set_color(GREEN);
            continue;
        }
        entity->set_vy(entity->get_vy() + GRAVITY * dt * SPEED);
        if (entity->get_vy() > MAX_FALL_SPEED) entity->set_vy(MAX_FALL_SPEED);
        entity->set_y(entity->get_y() + entity->get_vy() * dt * SPEED);
        entity->set_x(entity->get_x() + entity->get_vx() * dt * SPEED);

        if (entity->get_y() + entity->get_radius() >= HEIGHT) {
            entity->set_y(HEIGHT - entity->get_radius());
            entity->set_vy(-entity->get_vy() * BOUNCE);
            if (std::abs(entity->get_vy()) < 0.3) {
                entity->set_vy(0.0);
                entity->set_color(PURPLE);
            }
        }
    }
}

void PhysicsEffects::addToEntityList(Entity *entity){
    entity_ptr.push_back(entity);
}
