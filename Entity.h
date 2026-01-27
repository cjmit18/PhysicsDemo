#ifndef Entity_H
#define Entity_H

#include <string>
#include "raylib.h"

/**
 * @brief Simple circular entity used by the physics demo.
 *
 * The Entity encapsulates position, velocity, radius and simple state flags.
 * - Radius is used as a proxy for mass in collision resolution.
 * - Methods provide input handling, simple physics integration, and boundary checks.
 */
class Entity{
    
    private:
    std::string name{"Player"};
    Color color{GREEN};
    double x_pos{0.0};
    double y_pos{0.0};
    double z_pos{0.0};
    double radius{0.0};
    double vx{0.0};
    double vy{0.0};
    bool isColliding{false};
    bool isStatic{false};
    bool isOnGround{false};
    bool isAtCeiling{false};
    bool markedForDeletion{false};
    bool canMove{false};
    
    public:
    Entity() = default;
    /**
     * @brief Construct a new Entity
     * @param name Human-readable identifier (for debug)
     * @param x Initial X position
     * @param y Initial Y position
     * @param z Unused Z coordinate (reserved)
     * @param r Radius (also used as mass proxy)
     * @param c Draw color
     */
    Entity(const std::string &name, double x, double y, double z, double r, Color c);

    // Accessors and mutators
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
    void setCanMove(bool status);
    bool getCanMove() const; 

    // Input, bounds and physics helpers
    void checkInput(double gravity); ///< apply player inputs into velocity
    void checkBounds(int WIDTH, int HEIGHT); ///< clamp position to window and set collision flags
    void PhysicsEffect(int width, int height, double GRAVITY); ///< integrate velocities and apply gravity
    void setVelocity(double vx, double vy);
    void objectMovement(int WIDTH, int HEIGHT, double GRAVITY); ///< composite: input + physics + bounds

    // Collision / lifecycle flags
    bool getCollided() const;
    void setColliding(bool status);
    void markedForDeletionStatus(bool status);
    bool isMarkedForDeletion() const;
};
#endif // Entity_H