# Physics Demo — Bug Summary and Quick Fix Guide

This short README lists the most important issues found in the project and clear, prioritized fixes to make the physics stable and maintainable.

## Key Problems (short)
- **Time/units mismatch:** Inputs apply `SPEED_MULT` to velocities while physics integration multiplies by `dt * SPEED_MULT`. This mixes units and breaks clamping. See `inputManager.cpp` and `physicsEffects.cpp`.
- **Clamping mismatch:** `WALK_SPEED` and `MAX_WALK_SPEED` use different scales; clamping doesn't reliably limit speed. See `config.h` and `inputManager.cpp`.
- **Friction formula:** Using `std::pow(FRICTION, dt * SPEED_MULT)` mixes frame-rate scaling into the exponent; this causes unstable damping.
- **Memory & ownership:** Entities are stored as raw pointers and `players` is pre-sized to `MAX_ENTITIES` — this can lead to null slots, leaks, and double-registrations.
- **Collision edge-case:** When two entity centers overlap (distance == 0) resolution can divide by zero or apply incorrect corrections.
- **Duplicate registration:** `addToEntityList` pushes pointers without checking for duplicates.

## Priority Fixes (do these first)
1. Normalize units and integration
	- Store velocities as units/second. Remove `SPEED_MULT` from input velocity assignments.
	- Integrate positions using `position += velocity * dt` (apply `dt` once).
	- Files to edit: `inputManager.cpp`, `physicsEffects.cpp`, `config.h`.

2. Fix friction damping
	- Use `std::pow(FRICTION, dt)` or `exp(-k * dt)`; do not multiply `dt` by `SPEED_MULT`.

3. Unify speed constants
	- Ensure `WALK_SPEED` and `MAX_WALK_SPEED` are both per-second values (or both scaled). Adjust values and clamps.

4. Add collision safety
	- If distance == 0, apply a small separation vector (e.g., jitter) or skip positional impulse. Respect static flags.

## Lower-priority improvements
- Replace raw `Entity*` with `std::unique_ptr<Entity>` (or clearly state ownership). Convert `players` to an owning `vector`.
- Prevent duplicate registration in `addToEntityList` (check `std::find` before `push_back`).
- Normalize filename/include casing across the repo (ensure `#include` matches actual filenames).

## Quick verification checklist
- Run the game and confirm movement is smooth and max-speed clamps work.
- Confirm friction behaviour is consistent across frame rates.

## Next steps I can take
- Implement the top 3 fixes in a focused PR (units, friction, clamping).
- Convert entity ownership to `unique_ptr` and update registration functions.

Generated on 2026-01-31.
