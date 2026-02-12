# Physics Demo — Overview & Quick Fix Guide

A compact 2D circular-entity physics demo using raylib. This repository demonstrates simple per-entity physics (gravity, bounce, friction), pairwise collision resolution (positional correction + impulse), and per-entity input control.

**Quick Start**
- Requirements: A C++17-capable toolchain and raylib installed on your system.
- Example Windows g++ build (matches the provided VS Code build task):

```bash
C:\raylib\w64devkit\bin\g++.exe -fdiagnostics-color=always -g main.cpp Entity.cpp physicsEffects.cpp inputManager.cpp windowInteractions.cpp -o main.exe -IC:/raylib/include -LC:/raylib/lib -lraylib -lgdi32 -lwinmm
```

- After building, run the produced `main.exe`. The demo creates and simulates circular entities; use the keyboard to move, jump, and toggle entity flags.

**Files of interest**
- `main.cpp` — program entry, main loop, collision detection/resolution, entity spawn logic.
- `Entity.h` / `Entity.cpp` — Entity data and simple accessors/flags.
- `physicsEffects.h` / `physicsEffects.cpp` — gravity, per-frame friction, and bounce handling.
- `inputManager.h` / `inputManager.cpp` — maps keyboard input to entity velocity/flags.
- `windowInteractions.h` / `windowInteractions.cpp` — clamps entities to screen bounds and sets boundary flags.
- `config.h` — project-wide tuning constants (speeds, gravity, friction, radius limits).

**What this project implements**
- Continuous integration of velocity: positions updated with `position += velocity * dt`.
- Gravity, bounce and friction with per-frame clamping and safety checks.
- Pairwise collision resolution with positional correction and impulse-based velocity change.
- Simple input handling for movement, jump, toggle bounciness/static, and debug actions.

**Known issues & design notes**
- Managers hold non-owning raw pointers and the players pool is pre-sized; this requires care when spawning/deleting entities to avoid stale pointers.
- Edge-case collisions (centers overlapping) are handled with safe fallbacks to avoid NaNs; collision corrections are clamped per-step.
- Friction and damping use per-frame decay derived from a per-second retention factor to reduce frame-rate sensitivity.

**What was updated**
- Clarified an exact example build command for Windows with raylib (matches the VS Code task in the workspace).
- Added accurate file list and short description for each major source file.
- Reworded problem/priorities sections to match the code's current handling (per-frame integration, clamping, manager registration checks).

**Verification checklist**
- Runs and simulates at different frame rates with smooth movement (try 30 / 60 / 144 FPS).
- No NaNs or crashes when entities spawn at identical positions.
- Entities do not become permanently stuck in walls or floor after bounce/friction resolution.
- Repeated spawn/delete cycles do not steadily leak memory or leave dangling pointers (manual verification advised).

Generated: 2026-02-01
