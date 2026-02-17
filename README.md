# reflex-engine ⚙️

**reflex-engine** is a custom-built 2D physics engine written from scratch, using **raylib** as its rendering framework. It focuses on simulating realistic rigid body dynamics with an emphasis on high performance.

The engine simulates gravity, collision detection, and impulse-based collision resolution with support for **Friction** and **restitution**, making it suitable for both learning and demonstration purposes. It also ships with a collection of interactive demos and a sandbox mode for hands-on exploration.

---

## Features 📝

- **Rigid Body Dynamics**
  - Linear motion and force integration
  - Gravity and mass-based behavior

- **Collision Detection**
  - OBB/AABB/Sphere collision
  - SAT based overlap detection
  - .svg custom colliders (WIP)

- **Collision Resolution**
  - Impulse-based collision responses
  - Incorperates Friction (static & dynamic)
  - Incorperates Restitution (bounciness)

- **Rendering with raylib**
  - Real-time visualization
  - Simple, lightweight rendering layer

- **Demos & Sandbox**
  - Practical demonstrations showcasing engine features
  - Sandbox mode for freeform experimentation and testing

- **Architecture**
  - Application state controlled using layers system
  - Event propagation through layer stack

---

## Build
Project files and build are generated using CMake. I reccommend creating a folder called build and running 
```
cmake ..
```
from that directory. This will generate relevant project files which you can then use to build and run.

---

## Preview
<p align="center"><img width="256" height="256" alt="BinomialDistribution" src="https://github.com/user-attachments/assets/5b918063-4c71-4a66-8468-cab1ac072d5f" />&nbsp&nbsp&nbsp&nbsp&nbsp<img width="256" height="256" alt="Demolition" src="https://github.com/user-attachments/assets/2a8ce0da-0eaa-4127-a337-49ac604996d7" /></p>

