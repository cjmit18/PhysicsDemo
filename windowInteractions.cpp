// Implementation of windowInteractions: clamps Entities to screen bounds and updates flags/colors.

#include "windowInteractions.h"
#include "raylib.h"
#include "Entity.h"
#include "config.h"
#include <cmath>
#include <algorithm>


void windowInteractions::addToEntityList(Entity* e) {
    for (size_t i = 0; i < entity_ptr.size(); i++) {
        if (entity_ptr[i] == e) {
            return; // already in list
        }
        if (entity_ptr[i] == nullptr) {
            entity_ptr[i] = e; // added in empty slot
            return;
        }
    }
    // If we reach here, no empty slot was found; add to the end
    entity_ptr.push_back(e);
}
void windowInteractions::removeFromEntityList(Entity* e) {
    entity_ptr.erase(std::remove(entity_ptr.begin(), entity_ptr.end(), e), entity_ptr.end());
}
void windowInteractions::checkAllBounds() {

    for (auto& entity : entity_ptr) {
        int width = GetScreenWidth();
        int height = GetScreenHeight();
        if (!entity) {
            continue;
        }
        // default color for debug before any special flags are applied
        entity->set_color(RED);

        // Ensure radius never exceeds sensible half-screen limits (keeps in-bounds logic safe)
        if (entity->get_radius() >= width/2.0 || entity->get_radius() >= height/2.0) {
            entity->set_radius(std::min(width/2.0 , height/2.0));
        }

       if (entity->get_radius() >= MAX_RADIUS) {
            entity->set_radius(MAX_RADIUS);
        } 
         if (entity->get_radius() <= MIN_RADIUS) {
                entity->set_radius(MIN_RADIUS);
          }
        // Check bottom boundary
        if (entity->get_y() + entity->get_radius() >= height) {
            entity->set_y(height - entity->get_radius());
            entity->setOnGround(true);
        }
        // Check top boundary
        if (entity->get_y() - entity->get_radius() <= 0) {
            entity->set_y(entity->get_radius());
            entity->setAtCeiling(true);
        }
        // Check right boundary
        if (entity->get_x() + entity->get_radius() >= width) {
            entity->set_x(width - entity->get_radius());
            entity->setAtRight(true);
        }
        // Check left boundary
        if (entity->get_x() - entity->get_radius() <= 0) {
            entity->set_x(entity->get_radius());
            entity->setAtLeft(true);
        }
        // Ordering of flag checks below determines debug color precedence.
        // For example: collision (BLUE) may be overridden by ground (GREEN) etc.
        if (entity->getCollided()) {
            entity->set_color(BLUE);
        }
        if (entity->getOnGround()) {
            entity->set_color(GREEN);
        }
        if (entity->getAtCeiling()) {
            entity->set_color(YELLOW);
            entity->set_vy(0.0); // stop upward movement when at ceiling
        }
        if (entity->getAtLeft() || entity->getAtRight()) {
            entity->set_color(PURPLE);
            // Only stop horizontal movement for non-bouncy entities;
            // bouncy entities rely on physicsEffects to reflect velocity.
            if (!entity->getEntityBouncy()) {
                entity->set_vx(0.0); // stop horizontal movement
            }
        }
}
}