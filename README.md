# Rise of the AI – CS-UY 3113

**Author:** Johann Varghese
**Course:** CS-UY 3113
**Assignment:** Rise of the AI
**Date Due:** 04/04/2026

---

## Description

This project is a platformer game built using Raylib in C++.

The player controls Krabby, a crab on a beach quest, navigating through
3 levels of increasing difficulty while avoiding enemies and managing lives.

The game features:

- 3 fully scrolling levels with increasing difficulty
- Menu, Win, and Lose scenes as separate Scene objects
- 3 types of AI enemies: Wanderer, Follower, and Lerper
- Lives system shared across all levels
- Fully animated player character
- Looping background music and 3 sound effects
- Fixed timestep physics with gravity and collision detection
- Camera that follows the player horizontally

---

## Controls

| Key | Action |
|-----|--------|
| A / D | Move left / right |
| W | Jump |
| ENTER | Start game / Return to menu |
| 1 | Debug: jump to Level 1 |
| 2 | Debug: jump to Level 2 |
| 3 | Debug: jump to Level 3 |
| Close Window | Exit Game |

---

## Levels

| Level | Enemies | Difficulty |
|-------|---------|------------|
| Level 1 | 2 Wanderers | Easy |
| Level 2 | 2 Followers + 1 Wanderer | Medium |
| Level 3 | 1 Wanderer + 2 Followers + 1 Lerper | Hard |

---

## Features Implemented

- Scene system with Menu, 3 Levels, Win, and Lose scenes
- Fixed timestep physics loop
- Gravity and jump-based movement
- AABB collision detection with map probe system
- 3 AI types: Wanderer, Follower (FSM), Lerper (linear interpolation)
- Lives system (3 lives, shared across levels)
- Camera panning that follows player horizontally
- Looping background music per level
- Sound effects for jumping, taking damage, and dying
- Delta time for all movement

---

## How to Run

In the project directory:
```bash
make
./raylib_app
```
