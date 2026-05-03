Quasar Simulator

A real-time 3D quasar simulation built with OpenGL. Simulates the key 
visual features of a quasar: an accretion disk with Keplerian orbital 
mechanics, relativistic jets, and a central black hole.

<img width="1621" height="1264" alt="Screenshot 2026-05-02 234030" src="https://github.com/user-attachments/assets/fc26397e-1bc5-450e-9267-10d71028bf4a" />


Features

- Accretion disk: 25,000 instanced particles orbiting with 
  Keplerian mechanics (inner particles orbit faster than outer ones).
  Temperature based coloring from blue/white (inner) to orange/red (outer).
- Relativistic jets: Two particle streams shooting along the 
  magnetic axis with height-based fade
- Additive blending: Particles add light together, simulating 
  plasma emission
- Interactive camera: Mouse to rotate, scroll to zoom
- FPS counter: Displayed in window title

Stack

- C++17
- OpenGL 3.3 Core Profile
- GLFW: window creation and input
- GLAD: OpenGL function loading
- GLM: math library
- STB Image: texture loading

Build Instructions

Prerequisites

bash
sudo apt-get install libglfw3-dev libglm-dev libassimp-dev


Clone and build
bash
git clone https://github.com/mdiallo64/quasar_sim.git
cd quasar_sim
mkdir build && cd build
cmake ..
make
./quasar_sim


Implementation Notes

Instanced rendering: The accretion disk uses glDrawArraysInstanced 
to render 25,000 particles in one single draw call. Each instance has its own
orbital radius and starting angle that is stored in a per-instance VBO.

Keplerian orbital mechanics: Angular velocity is calculated by 
ω = 1 / √(r³) in the vertex shader, so inner particles 
complete orbits faster than outer ones.

Additive blending: glBlendFunc(GL_SRC_ALPHA, GL_ONE) makes 
overlapping particles add their light contributions together rather 
than occluding each other, simulating plasma emission.

Temperature coloring: Each particle's color is interpolated 
between blue/white and orange/red based on its normalized orbital radius.

