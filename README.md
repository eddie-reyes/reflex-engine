# reflex-engine ⚙️

**reflex-engine** is a custom-built 2D physics engine written from scratch, using **raylib** as its rendering framework. It focuses on simulating realistic rigid body dynamics with an emphasis on high performance.

The engine simulates gravity, collision detection, and impulse-based collision resolution with support for **friction** and **restitution**, making it suitable for both learning and demonstration purposes. It also ships with a collection of interactive demos and a sandbox mode for hands-on exploration.

---

## Features 📝

- **Rigid Body Dynamics**
  - Linear motion and force integration
  - Gravity and mass-based behavior

- **Collision Detection**
  - Inherited from raylib
  - Box/Sphere collision
  - .svg custom colliders (WIP)

- **Collision Resolution**
  - Impulse-based collision response
  - Friction (static & dynamic)
  - Restitution (bounciness)

- **Rendering with raylib**
  - Real-time visualization
  - Simple, lightweight rendering layer

- **Demos & Sandbox**
  - Practical demonstrations showcasing engine features
  - Sandbox mode for freeform experimentation and testing

---

## Build
Project files and build are generated using CMake. I reccommend creating a folder called build and running 
```
cmake ..
```
from that directory. This will generate relevant project files which you can then use to build and run.
