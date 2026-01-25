#ifndef Entity_H
#define Entity_H

#include <string>
#include "raylib.h"

class Entity{
    
    private:
    std::string name;
    double x_pos{0.0};
    double y_pos{0.0};
    double z_pos{0.0};
    double radius{0.0};
    double vx{0.0};
    double vy{0.0};
    Color color{GREEN};
    bool isColliding{false}; 
    
    public:
    Entity() = default;
    Entity(const std::string &name, double x, double y, double z, double r, Color c);

    std::string get_name() const;
    void set_name(const std::string &new_name);
    double get_x() const;
    void set_x(double x);
    double get_y() const;
    void set_y(double y);
    double get_radius() const;
    void set_radius(double r);
    double get_vx() const;
    double get_vy() const;
    void set_vx(double vx);
    void set_vy(double vy);
    void set_color(Color color);
    Color get_color() const;
    void checkInput();
    void checkBounds(int WIDTH, int HEIGHT);
    void GravityEffect(int width, int height, double GRAVITY);
    void setVelocity(double vx, double vy);
    void objectMovement(int WIDTH, int HEIGHT, double GRAVITY);
    bool getCollided() const;
    void setColliding(bool status);
    

};
#endif // Entity_H