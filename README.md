# Physics Demo — Overview & Quick Fix Guide

A minimal 2D circular-entity physics demo using raylib. This document summarizes how to run the project, the most important issues observed, prioritized fixes to improve stability, and a short verification checklist.

## Quick start
- Requirements: C++17, raylib.
- Build: use your usual build system (e.g., CMake or a simple g++ command).
- Run: launch the produced executable; window is resizable and controlled by keyboard.

## What this project does
Simulates simple circular entities with:
- gravity, bounce and friction
- basic collision resolution (position correction + impulse)
- per-entity input control (move, jump, toggle bouncy/static)

## Key problems (high level)
- Raw pointer ownership and pre-sized player pool cause null slots, leaks, and duplicated registrations.
- Collision edge-case when centers overlap (distance == 0) can produce NaNs or unstable corrections.
- Inconsistent units and friction semantics cause frame-rate dependent behaviour.
- Duplicate registration into manager lists can cause redundant work and stale pointers.

## Priority fixes (apply in order)
1. Stabilize integration and units
   - Ensure velocities are pixels/second and integrate with position += velocity * dt.
   - Verify WALK/FLY/FALL speeds and MAX_* clamps are consistent per-second values.
2. Fix friction and damping
   - Use per-frame decay derived from a per-second retention (e.g., pow(FRICTION, dt) or exp(-k*dt)).
3. Collision safety
   - Handle zero-distance by using relative-velocity fallback or deterministic jitter; clamp positional correction per-step.
4. Ownership & registration
   - Prevent duplicate registrations (check before push_back).
   - Consider using unique_ptr for players or clearly document ownership and ensure single responsible owner.
5. Clean-up and deletion
   - Remove entities from all managers before delete; prefer marking-for-deletion then cleaning in a single place.

## Verification checklist
- Movement is smooth at different frame rates (run at 30/60/144 FPS).
- No NaNs or crashes when entities spawn at same position.
- Entities do not become permanently stuck in walls or floor.
- Memory usage stable when spawning/deleting entities repeatedly.

## Recommended next steps
- Implement items 1–3 in a focused PR and run automated playtests.
- Optional: migrate players container to owning smart pointers and ensure managers hold non-owning raw pointers or weak references.
- Add unit tests for collision resolve (including zero-distance case) and for per-frame friction behaviour.

Generated: 2026-01-31
