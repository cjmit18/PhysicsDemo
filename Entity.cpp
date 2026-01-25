#include "Entity.h"
#include "raylib.h"

#define WIDTH GetScreenWidth()
#define HEIGHT GetScreenHeight()

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
double Entity::checkInput(){
    if (IsKeyDown(KEY_RIGHT)) {
        this->set_x(this->x_pos + 1.0);
    }
    if (IsKeyDown(KEY_LEFT)) {
        this->set_x(this->x_pos - 1.0);
    }
    if (IsKeyDown(KEY_UP)) {
        this->set_y(this->y_pos - 1.0);
    }
    if (IsKeyDown(KEY_DOWN)) {
        this->set_y(this->y_pos + 1.0);
    }
    if (IsKeyDown(KEY_EQUAL)) {
        this->set_radius(this->radius + 1.0);
    }
    if (IsKeyDown(KEY_MINUS)) {
        this->set_radius(this->radius - 1.0);
        if (this->get_radius() < 100.0) {
            this->set_radius(100.0);
        }
    }
    return 0;
}
int Entity::checkBounds() {
    double maxRadius = std::min(WIDTH/2 , HEIGHT/2);
    if (this->get_radius() > maxRadius) {
        this->set_radius(maxRadius);
    }
    if (this->get_x() - this->get_radius() < 0) { // Left bound
        this->set_x(this->get_radius());
    }
    if (this->get_x() + this->get_radius() > 800) { // Right bound
        this->set_x(WIDTH - this->get_radius());
    }
    if (this->get_y() - this->get_radius() < 0) { // Top bound
        this->set_y(this->get_radius());
    }
    if (this->get_y() + this->get_radius() > 600) { // Bottom bound
        this->set_y(HEIGHT - this->get_radius());
    }
    return 0;
}