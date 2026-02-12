// inputManager implementation: converts keyboard state into per-entity velocity updates.
// Notes: uses dt = GetFrameTime(); WALK_SPEED/FLYSPEED treated as per-second accelerations/impulses.

#include "Entity.h"
#include "raylib.h"
#include "inputManager.h"
#include "config.h"
#include "commands.h"
#include <vector>
#include <cmath> // added for std::abs
#include <algorithm>


void inputManager::addToEntityList(Entity *entity){
    if (std::find(entity_ptr.begin(), entity_ptr.end(), entity) != entity_ptr.end()) {
        return; // already in list
    }
    if (auto it = std::find(entity_ptr.begin(), entity_ptr.end(), nullptr); it != entity_ptr.end()) {
        *it = entity; // added in empty slot
        return;
    }
    // If we reach here, no empty slot was found; add to the end
    entity_ptr.push_back(entity);
}
void inputManager::removeFromEntityList(Entity *entity) {
    entity_ptr.erase(std::remove(entity_ptr.begin(), entity_ptr.end(), entity), entity_ptr.end());
}
void inputManager::processInputs(){
    float dt = GetFrameTime();
    // Iterate over a snapshot to avoid iterator invalidation if `SpawnEntity`
    // or other calls modify `entity_ptr` during processing.
    std::vector<Entity*> snapshot = entity_ptr;
    for (Entity *entity : snapshot) {
        if (!entity) continue;
        if (!entity->getCanMove()) continue;

        // Horizontal: apply acceleration scaled by 1/mass and clamped to MAX_WALK_SPEED.
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
        // Jumping: instant velocity impulse for simplicity (FLYSPEED interpreted as initial jump speed).
        if (IsKeyPressed(KEY_SPACE)) {
            if (entity->getOnGround()) {
                double mass = entity->getWeight(); if (mass <= 0.0) mass = 1.0;
                entity->set_vy(-FLYSPEED / mass); // instant jump impulse
                entity->setOnGround(false);
            }
        }
        // When no horizontal input, apply damping using same friction semantics as physicsEffects.
        if (entity->getCanMove() && !(IsKeyDown(KEY_D) || IsKeyDown(KEY_A))) {
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
        if (IsKeyPressed(KEY_B)) {
            entity->setEntityBouncy(!entity->getEntityBouncy());
        }
        if (IsKeyPressed(KEY_F)) {
            // Toggle fullscreen OR toggle borderless windowed mode (separately)
            static bool borderless = false;
            // Toggle fullscreen first if desired (uncomment if you want fullscreen toggle)
            // ToggleFullscreen();

            // Toggle borderless windowed mode without forcing it every frame
            borderless = !borderless;
            if (borderless) {
                SetWindowState(FLAG_BORDERLESS_WINDOWED_MODE);
            } else {
                ClearWindowState(FLAG_BORDERLESS_WINDOWED_MODE);
            }
        }
        if (IsKeyPressed(KEY_V)) {
            int width[] = {1280,1920,2560,3840};
            int height[] = {720,1080,1440,2160};
            int monW = GetMonitorWidth(GetCurrentMonitor());
            int monH = GetMonitorHeight(GetCurrentMonitor());
            for (size_t i = 0; i < 4; ++i) {
                if (GetScreenWidth() == width[i] && GetScreenHeight() == height[i]) {
                    int newIndex = (i + 1) % 4;
                    // Only change if the next resolution fits the current monitor
                    if (width[newIndex] <= monW && height[newIndex] <= monH) {
                        SetWindowSize(width[newIndex], height[newIndex]);
                    }
                    if (newIndex == 0 && (monW < width[0] || monH < height[0])) {
                        // If even the smallest resolution doesn't fit, set to monitor size
                        SetWindowSize(monW, monH);
                    }
                    break;
                }
            }
        }
        if ((IsKeyPressed(KEY_B))) {
            SpawnEntity(entity->get_x() + 50, entity->get_y() + 50, entity->get_radius(), entity->getWeight(), entity->get_color(), 1);
        }
    }

}