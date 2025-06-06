# MagDula-Ca-Og-Volleyball

A simple two-player local multiplayer volleyball game built with SFML 2.6. The game features realistic ball physics, animated characters, and a first-to-five scoring system.

---
![image](https://github.com/user-attachments/assets/8738b15a-2e3c-4e63-8c10-827e8e56115e)


## Features

* **Two-player local multiplayer** volleyball action.
* **Realistic ball physics** with gravity and bouncing.
* **Animated player sprites** with idle, running, and jumping states.
* **Sound effects** for ball interactions and scoring.
* **Score tracking system**: The first player to score 5 points wins.
* **Net collision detection**.
* **Boundary collision system** to keep the ball in play.
* **Speed boost** mechanic for faster movement.
* **Responsive controls** with support for simultaneous key presses.

---

## Installation and Compilation

### Prerequisites
* A C++ compiler (e.g., MSVC, GCC)
* Visual Studio (recommended, as the project files are included)

### Setup
1.  **Download SFML 2.6.x** from the official website: [https://www.sfml-dev.org/download.php](https://www.sfml-dev.org/download.php)
2.  **Extract** the archive to a directory on your system (e.g., `C:\SFML-2.6.x`).
3.  **Configure Environment Variables**:
    * Add the SFML `bin` directory (e.g., `C:\SFML-2.6.x\bin`) to your system's `PATH`.
    * Set the `SFML_ROOT` environment variable to your SFML directory path.
    * Set the `SFML_DIR` environment variable to the CMake path within your SFML directory (e.g., `C:\SFML-2.6.x\lib\cmake\SFML`).

### Compilation
1.  Clone or download this repository.
2.  Open the `balibol.vcxproj` file with Visual Studio.
3.  Ensure the project settings in Visual Studio are correctly linking to your SFML include and lib directories.
4.  Build the solution in either **Debug** or **Release** mode.
5.  The executable will be generated in the `Debug` or `Release` folder.

---

## Game Controls

### Player 1 (Left Side)
* **A**: Move left
* **D**: Move right
* **W**: Jump
* **Space**: Speed boost (hold while moving)

### Player 2 (Right Side)
* **Left Arrow / Numpad 4**: Move left
* **Right Arrow / Numpad 6**: Move right
* **Up Arrow / Numpad 8**: Jump
* **Right Shift**: Speed boost (hold while moving)

---

## Gameplay

### Objective
Be the first player to score **5 points**. A point is scored when the ball lands on the opponent's side of the court.

### Rules
* The ball must travel over the net for a point to be scored.
* Players can hit the ball to influence its direction and velocity.
* The first player to reach 5 points wins the match.

### Mechanics
* **Ball Physics**: The ball is influenced by gravity and will bounce off surfaces while maintaining its momentum.
* **Player Collision**: When a player touches the ball, the ball's velocity is updated based on the player's movement.
* **Net Collision**: The ball realistically bounces off the net.
* **Boundary Collision**: The ball will bounce off the edges of the screen.
* **Simultaneous Controls**: Players can move horizontally and jump at the same time for fluid control.
