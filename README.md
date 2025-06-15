# Spaceship

**Spaceship** is an OpenGL project that simulates a spaceship flying through space.
The scene includes a dynamic environment where the user controls a spaceship in real time. It also features one static object — a space station — placed at the center of the scene.
The project demonstrates basic 3D transformations, camera controls, and user interaction using keyboard input.

## Features

- Control the spaceship using `W`, `S`, `A`, and `D` keys to move forward, backward, and rotate.
- Two camera modes:
  - **Fixed camera** that stays static.
  - **Follow camera** that stays above and behind the ship, tracking its movement.
- Switch between camera views by pressing the `C` key.

# Setup & Run

To run this project, make sure the required dependencies are installed. On Fedora, you can install them with:

```bash
sudo dnf install cmake mesa-libGL glm-devel
```

Once dependencies are installed, simply run the provided build script:

```bash
bash build.sh
```
