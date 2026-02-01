#include "Entity.h"
#include "raylib.h"
#include <cmath>

// Physics tuning constants:
// - BOUNCE: restitution multiplier applied on ground contact (0 => no bounce).
// - FALL_SPEED: additional downward acceleration when holding S.
// - MAX_FALL_SPEED: clamp for downward velocity to avoid runaway.
// - FLYSPEED: instantaneous upward velocity when W is pressed.
// - WALK_SPEED / MAX_WALK_SPEED: horizontal control responsiveness/clamp.

Entity::Entity(const std::string &name, double x, double y, double z, double r, Color c) : name(name), x_pos(x), y_pos(y),z_pos(z), radius(r), color(c) {}


void Entity::showInfo(){
    // Debug function to print entity info to console
    DrawText(("Entity: " + this->get_name()).c_str(), 10, 10, 10, BLACK);
    DrawText(("Position: (" + std::to_string(this->get_x()) + ", " + std::to_string(this->get_y()) + ")").c_str(), 10, 25, 10, BLACK);
    DrawText(("Velocity: (" + std::to_string(this->get_vx()) + ", " + std::to_string(this->get_vy()) + ")").c_str(), 10, 40, 10, BLACK);
    DrawText(("Radius: " + std::to_string(this->get_radius())).c_str(), 10, 55, 10, BLACK);
}
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

void Entity::addToVx(double dvx) {
    this->vx += dvx; // accumulate change in velocity
}
void Entity::addToVy(double dvy) {
    this->vy += dvy;
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
void Entity::setColliding(bool status) {
    isColliding = status;
}
bool Entity::getCollided() const {
    return isColliding;
}
bool Entity::getOnGround() const{
    return isOnGround;
}
void Entity::setOnGround(bool status){
    isOnGround = status;
}
bool Entity::getAtCeiling() const{
    return isAtCeiling;
}
void Entity::setAtCeiling(bool status){
    isAtCeiling = status;
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
void Entity::resetFlags() {
    isColliding = false;
    isOnGround = false;
    isAtCeiling = false;
}