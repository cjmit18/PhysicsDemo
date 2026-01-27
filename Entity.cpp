#include "Entity.h"
#include "raylib.h"
#include <cmath>

// Physics tuning constants:
// - BOUNCE: restitution multiplier applied on ground contact (0 => no bounce).
// - FALL_ACCEL: additional downward acceleration when holding S.
// - MAX_FALL_SPEED: clamp for downward velocity to avoid runaway.
// - FLYSPEED: instantaneous upward velocity when W is pressed.
// - WALK_ACCEL / MAX_WALK_SPEED: horizontal control responsiveness/clamp.
#define BOUNCE 0.0
#define FALL_ACCEL 1.0
#define MAX_FALL_SPEED 10.0
#define FLYSPEED -1.5
#define WALK_ACCEL 1.0
#define MAX_WALK_SPEED 5.0
#define MAX_RADIUS 5.0
#define MIN_RADIUS 1.0

Entity::Entity(const std::string &name, double x, double y, double z, double r, Color c) : name(name), x_pos(x), y_pos(y),z_pos(z), radius(r), color(c) {}

std::string Entity::get_name() const {
        return name;
    }


void Entity::set_name(const std::string &new_name) {
    name = new_name;
    }
double Entity::get_x() const {
    return x_pos;
    }
void Entity::set_x(double x) {
    x_pos = x;
    }
double Entity::get_y() const {
    return y_pos;
    }
void Entity::set_y(double y) {
    y_pos = y;
    }
double Entity::get_radius() const {
    return radius;
    }
void Entity::set_radius(double r) {
    radius = r;
    }
void Entity::set_vx(double vx) {
    this->vx = vx;
}
void Entity::set_vy(double vy) {
    this->vy = vy;
}

void Entity::setVelocity(double vx, double vy) {
    this->vx = vx; // assign instead of accumulate
    this->vy = vy;
}
double Entity::get_vx() const {
    return vx;
}
double Entity::get_vy() const {
    return vy;
}

void Entity::set_color(Color color) {
    this->color = color;
}
Color Entity::get_color() const {
    return color;
}
void Entity::objectMovement(int WIDTH, int HEIGHT, double gravity) {
    // Composite step: read input, apply physics integration, enforce bounds.
    this->checkInput(gravity);
    this->PhysicsEffect(WIDTH, HEIGHT, gravity);
    this->checkBounds(WIDTH, HEIGHT);
}

void Entity::setColliding(bool status) {
    isColliding = status;
}

bool Entity::getCollided() const {
    return isColliding;
}
bool Entity::isMarkedForDeletion() const {
    return markedForDeletion;
}
void Entity::markedForDeletionStatus(bool status) {
    markedForDeletion = status;
}
void Entity::setCanMove(bool status) {
    canMove = status;
}
bool Entity::getCanMove() const {
    return canMove;
}
void Entity::checkInput(double gravity){
    // Horizontal: apply acceleration so input can overcome collision impulses
    // Damping when no input helps objects come to rest.
    if (getCanMove()) {
        if (IsKeyDown(KEY_D)) {
            this->set_vx(this->get_vx() + WALK_ACCEL);
            if (this->get_vx() > MAX_WALK_SPEED) {
                this->set_vx(MAX_WALK_SPEED);
            }
        } 
        else if (IsKeyDown(KEY_A)) {
            this->set_vx(this->get_vx() - WALK_ACCEL);
                if (this->get_vx() < -MAX_WALK_SPEED) {
                    this->set_vx(-MAX_WALK_SPEED);
                }
            }
        // Vertical: treat as velocity impulses instead of position edits
        if (IsKeyDown(KEY_W)) {
            this->set_vy(FLYSPEED);
        }
        if (IsKeyDown(KEY_S)) {
            // apply fall acceleration as frame-rate independent increment
            this->set_vy(this->get_vy() + (FALL_ACCEL * GetFrameTime()));
        }

        if (IsKeyDown(KEY_EQUAL)) {
            this->set_radius(this->get_radius() + 1.0);
            if (this->get_radius() > MAX_RADIUS) {
                this->set_radius(MAX_RADIUS);
            }
        }
        if (IsKeyDown(KEY_MINUS)) {
            this->set_radius(this->get_radius() - 1.0);
            if (this->get_radius() < MIN_RADIUS) {
                this->set_radius(MIN_RADIUS);
            }
        }
        if (IsKeyDown(KEY_DELETE)) {
            this->markedForDeletionStatus(true);
            }
        }
        if ((!getCanMove() && !(IsKeyDown(KEY_D) || IsKeyDown(KEY_A)))) {
            this->set_vx(this->get_vx() * 0.8);
            if (std::abs(this->get_vx()) < 0.05){
                this->set_vx(0.0);
            }
        }
}
void Entity::checkBounds(int WIDTH, int HEIGHT){
    // Enforce window boundaries and set collision/on-ground flags.
    // When hitting the top, vertical velocity is zeroed; hitting bottom may set isOnGround.
    // A radius larger than screen half-size is clamped and treated as colliding.
    double maxRadius = std::min(WIDTH/2.0 , HEIGHT/2.0);
    if (this->get_radius() > maxRadius) {
        this->set_radius(maxRadius);
        this->isColliding = true;
    }
    if (this->get_x() - this->get_radius() < 0) { // Left bound
        this->set_x(this->get_radius());
        this->isColliding = true;
    }
    if (this->get_x() + this->get_radius() > WIDTH) { // Right bound
        this->set_x(WIDTH - this->get_radius());
        this->isColliding = true;
    }
    if (this->get_y() - this->get_radius() < 0) { // Top bound
        this->set_y(this->get_radius());
        this->set_vy(0.0); // stop upward movement
        this->isAtCeiling = true;
        this->isColliding = true;
    }
    if (this->get_y() + this->get_radius() >= HEIGHT) { // Bottom bound
        this->set_y(HEIGHT - this->get_radius());
        // if still moving vertically consider it a collision, otherwise treat as resting
        if (std::abs(this->get_vy()) > 1e-3) {
            this->isColliding = true;
            this->isOnGround = true;
        }
    }
     if (getCollided()) {
        this->set_color(BLUE);
    }
 }

void Entity::PhysicsEffect(int WIDTH, int HEIGHT, double GRAVITY){
    // If entity is effectively at rest on the ground, skip re-applying gravity and only
    // update horizontal position. This prevents jitter when vy is near zero.
    if (std::abs(this->get_vy()) < 1e-6 && std::abs((this->get_y() + this->get_radius()) - HEIGHT) < 1e-6) {
        this->set_x(this->get_x() + this->get_vx()); // update horizontal position
        this->set_color(GREEN);
        return;
    }
    // Simple Euler integration using GetFrameTime() for frame-rate independence.
    this->set_vy(this->get_vy() + (GRAVITY*GetFrameTime())); // apply gravity
    if (this->get_vy() > MAX_FALL_SPEED) this->set_vy(MAX_FALL_SPEED);
    this->set_y(this->get_y() + this->get_vy()); // update vertical position
    this->set_x(this->get_x() + this->get_vx()); // update horizontal position

    // Ground collision handling: snap to ground and apply restitution. Small bounces are suppressed.
    if (this->get_y() + this->get_radius() >= HEIGHT) { // hit the ground
        this->set_y(HEIGHT - this->get_radius());
        this->set_vy(-this->get_vy() * BOUNCE); // reverse and reduce velocity
        if (std::abs(this->get_vy()) < 0.3) {
            this->set_vy(0.0);
            this->set_color(PURPLE);
        }
    }
}
