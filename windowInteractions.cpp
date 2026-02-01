#include "windowInteractions.h"
#include "raylib.h"
#include "Entity.h"
#include "config.h"
#include <cmath>
#include <algorithm>


void windowInteractions::addToEntityList(Entity* e) {
    entity_ptr.push_back(e);
}
void windowInteractions::removeFromEntityList(Entity* e) {
    entity_ptr.erase(std::remove(entity_ptr.begin(), entity_ptr.end(), e), entity_ptr.end());
}
void windowInteractions::checkAllBounds() {
    for (auto& entity : entity_ptr) {

        if (!entity) {
            continue;
        }

        entity->set_color(RED);

        if (entity->get_radius() >= WIDTH/2.0 || entity->get_radius() >= HEIGHT/2.0) {
            entity->set_radius(std::min(WIDTH/2.0 , HEIGHT/2.0));
        }

       if (entity->get_radius() >= MAX_RADIUS) {
            entity->set_radius(MAX_RADIUS);
        } 
         if (entity->get_radius() <= MIN_RADIUS) {
                entity->set_radius(MIN_RADIUS);
          }
        // Check bottom boundary
        if (entity->get_y() + entity->get_radius() >= HEIGHT) {
            entity->set_y(HEIGHT - entity->get_radius());
            entity->setOnGround(true);
        }
        // Check top boundary
        if (entity->get_y() - entity->get_radius() <= 0) {
            entity->set_y(entity->get_radius());
            entity->setAtCeiling(true);
        }
        // Check right boundary
        if (entity->get_x() + entity->get_radius() >= WIDTH) {
            entity->set_x(WIDTH - entity->get_radius());
            entity->setAtRight(true);
        }
        // Check left boundary
        if (entity->get_x() - entity->get_radius() <= 0) {
            entity->set_x(entity->get_radius());
            entity->setAtLeft(true);
        }
        if (entity->getCollided()) {
            entity->set_color(BLUE);
        }
        if (entity->getOnGround()) {
            entity->set_color(GREEN);
        }
        if (entity->getAtCeiling()) {
            entity->set_color(YELLOW);
            entity->set_vy(0.0); // stop upward movement
        }
        if (entity->getAtLeft() || entity->getAtRight()) {
            entity->set_color(PURPLE);
            entity->set_vx(0.0); // stop horizontal movement
        }
}
}