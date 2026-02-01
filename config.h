// Project-wide physics and limits (units: pixels for positions/radius, pixels/s for velocities, pixels/s^2 for accelerations)
#ifndef config_H
#define config_H
#include "raylib.h"

#define MAX_ENTITIES 1000
#define SPEED_MULT 1.0

#define GRAVITY 10 // pixels per second squared
#define BOUNCE 0.9 // higher is bouncier lower is less bouncy
#define FRICTION 0.8
#define FALL_SPEED 10000.0
#define FLYSPEED 10000.0 
#define WALK_SPEED 10000.0
#define MAX_FLY_SPEED 15000.0
#define MAX_WALK_SPEED 15000.0
#define MAX_FALL_SPEED 15000.0


#define MAX_RADIUS 100.0
#define MIN_RADIUS 5.0

#endif // CONFIG_H