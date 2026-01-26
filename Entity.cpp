#include "Entity.h"
#include "raylib.h"
#include <cmath>
#define WALK_SPEED 1.0
#define BOUNCE 0.5
#define FALLSPEED 0.5
#define FLYSPEED -2.0

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
    this->checkInput(gravity);
    this->GravityEffect(WIDTH, HEIGHT, gravity);
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
void Entity::checkInput(double gravity){
    // Horizontal: control velocity, don't modify position directly
    if (IsKeyDown(KEY_D)) {
        this->set_vx(WALK_SPEED);
    } 
    else if (IsKeyDown(KEY_A)) {
        this->set_vx(-WALK_SPEED);
    } 
    else {
        // simple damping when no horizontal input
        this->set_vx(this->get_vx() * 0.8);
        if (std::abs(this->get_vx()) < 0.1) this->set_vx(0.0);
    }
    // Vertical: treat as velocity impulses instead of position edits
    if (IsKeyDown(KEY_W)) {
        this->set_vy(FLYSPEED);
    }
    if (IsKeyDown(KEY_S)) {
        this->set_vy(FALLSPEED);
    }

    if (IsKeyDown(KEY_EQUAL)) {
        this->set_radius(this->get_radius() + 1.0);
        if (this->get_radius() > 150.0) {
            this->set_radius(150.0);
        }
    }
    if (IsKeyDown(KEY_MINUS)) {
        this->set_radius(this->get_radius() - 1.0);
        if (this->get_radius() < 100.0) {
            this->set_radius(100.0);
        }
    }
    if (IsKeyDown(KEY_DELETE)) {
        this->markedForDeletionStatus(true);
        }
    }
void Entity::checkBounds(int WIDTH, int HEIGHT){
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
        this->isStatic = false;
    }
    if (this->get_y() + this->get_radius() > HEIGHT) { // Bottom bound
        this->set_y(HEIGHT - this->get_radius());
        // if still moving vertically consider it a collision, otherwise treat as resting
        if (std::abs(this->get_vy()) > 1e-3) {
            this->isColliding = true;
            this->isOnGround = false;
            this->isStatic = false;
        }
        else {
            this->isOnGround = true;
            this->isStatic = true;
        }
    }
    if (getCollided()) {
        this->set_color(BLUE);
    }
}

void Entity::GravityEffect(int WIDTH, int HEIGHT, double GRAVITY){
    // If resting on the ground, do not re-apply gravity
    if (std::abs(this->get_vy()) < 1e-6 && std::abs((this->get_y() + this->get_radius()) - HEIGHT) < 1e-6) {
        this->set_color(GREEN);
        return;
    }
    // simple Euler integration
    this->set_vy(this->get_vy() + GRAVITY); // apply gravity to vertical velocity
    this->set_y(this->get_y() + this->get_vy()); // update vertical position
    this->set_x(this->get_x() + this->get_vx()); // update horizontal position

    // ground collision handling
    if (this->get_y() + this->get_radius() >= HEIGHT) { // hit the ground
        this->set_y(HEIGHT - this->get_radius());
        this->set_vy(-this->get_vy() * BOUNCE); // reverse and reduce velocity
        // If the bounce is very small, consider it resting
        if (std::abs(this->get_vy()) < 0.3) {
            this->set_vy(0.0);
            this->set_color(GREEN);
        }
    }
}
