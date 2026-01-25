#ifndef Entity_H
#define Entity_H

#include <string>
#include "raylib.h"

class Entity{
    
    private:
    std::string name;
    double x_pos;
    double y_pos;
    double radius;
    
    public:
    Entity() = default;
    Entity(const std::string &name, double x, double y, double r);

    std::string get_name() const;
    void set_name(const std::string &new_name);
    double get_x() const;
    void set_x(double x);
    double get_y() const;
    void set_y(double y);
    double get_radius() const;
    void set_radius(double r);
    double checkInput();
    int checkBounds();

};
#endif // Entity_H