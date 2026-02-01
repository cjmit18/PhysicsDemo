#ifndef Entity_H
#define Entity_H
#include <string>
#include "raylib.h"
#include "config.h"
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
    double weight{0.0};
    bool isColliding{false};
    bool isStatic{false};
    bool isOnGround{false};
    bool isAtCeiling{false};
    bool isAtLeft{false};
    bool isAtRight{false};
    bool markedForDeletion{false};
    bool canMove{false};
    bool isBouncy{true};
    
    public:
    Entity() = default;
    /**
     * @brief Construct a new Entity
     * @param name Human-readable identifier (for debug)
     * @param x Initial X position
     * @param y Initial Y position
     * @param z Unused Z coordinate (reserved)
     * @param r Radius (also used as mass proxy)
     * @param weight Weight (mass)
     * @param c Draw color
     */
    Entity(const std::string &name, double x, double y, double z, double r, double weight, Color c);

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
    void setWeight(double weight);
    double getWeight() const;
    void setCanMove(bool status);
    bool getCanMove() const;
    void addToVx(double dvx);
    void addToVy(double dvy);

    // Input, bounds and physics helpers
    //void checkBounds(); ///< clamp position to window and set collision flags
    void setVelocity(double vx, double vy);
    void showInfo(); ///< debug: print entity info to console

    // Collision / lifecycle flags
    bool getCollided() const;
    void setColliding(bool status);
    bool getOnGround() const;
    bool getAtCeiling() const;
    void setOnGround(bool status);
    void setAtCeiling(bool status);
    void setAtLeft(bool status);
    bool getAtLeft() const;
    bool getAtRight() const;
    void setAtRight(bool status);
    void markedForDeletionStatus(bool status);
    bool isMarkedForDeletion() const;
    void resetFlags(); ///< reset collision and state flags each frame
    bool isEntityBouncy() const;
    void setEntityBouncy(bool status);
    void setStatic(bool status);
    bool getEntityStatic() const;
    void setIsbouncing(bool status);
    bool getIsbouncing() const;
};
#endif // Entity_H