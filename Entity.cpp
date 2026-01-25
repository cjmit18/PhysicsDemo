#include "Entity.h"
#include "raylib.h"
#include <cmath>
#define WALK_SPEED 1.0
#define BOUNCE 0.5
#define FALLSPEED 0.5
#define FLYSPEED -2.0

Entity::Entity(const std::string &name, double x, double y, double r): name(name), x_pos(x), y_pos(y), radius(r) {}

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
void Entity::objectMovement(int WIDTH, int HEIGHT, double GRAVITY) {
    this->checkInput();
    this->GravityEffect(WIDTH, HEIGHT, GRAVITY);
    this->checkBounds(WIDTH, HEIGHT);
}

void Entity::checkInput(){
    if (IsKeyDown(KEY_RIGHT)) {
        this->set_x(this->get_x() + WALK_SPEED);
    }
    if (IsKeyDown(KEY_LEFT)) {
        this->set_x(this->get_x() - WALK_SPEED);
    }
    if (IsKeyDown(KEY_UP)) {
        this->set_vy(FLYSPEED);
    }
    if (IsKeyDown(KEY_DOWN)) {
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
}
void Entity::checkBounds(int WIDTH, int HEIGHT){
    double maxRadius = std::min(WIDTH/2.0 , HEIGHT/2.0);
    if (this->get_radius() > maxRadius) {
        this->set_radius(maxRadius);
    }
    if (this->get_x() - this->get_radius() < 0) { // Left bound
        this->set_x(this->get_radius());
    }
    if (this->get_x() + this->get_radius() > WIDTH) { // Right bound
        this->set_x(WIDTH - this->get_radius());
    }
    if (this->get_y() - this->get_radius() < 0) { // Top bound
        this->set_y(this->get_radius());
    }
    if (this->get_y() + this->get_radius() > HEIGHT) { // Bottom bound
        this->set_y(HEIGHT - this->get_radius());
    }
    
}

void Entity::GravityEffect(int WIDTH, int HEIGHT, double GRAVITY){
    this->set_vy(this->get_vy() + GRAVITY);
    this->set_y(this->get_y() + this->get_vy());
        if (this->get_y() + this->get_radius() > HEIGHT) { // Ground collision
            this->set_y(HEIGHT - this->get_radius());
            this->vy = -this->vy * BOUNCE; // Reset vertical velocity on ground contact
        if (std::abs(this->get_vy()) < 0.1) {
            this->set_vy(0.0); // Stop small bounces
        }
    }
}
