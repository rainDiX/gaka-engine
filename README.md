# Gaka-cpp (画家)

An OpenGL 4.3+ Renderer written in Modern C++

![Gaka rendering a Bezier surface in a SDL2 Window](/.readme/bezier_surface.png?raw=true "Gaka rendering a Bezier surface")

## Build instructions
Build dependencies :
- Python3 and jinja2 module (for glad)
- CMake
- a C++17 compiler

Dependencies :
- OpenGL 4.6
- glm
- SDL2

```sh
git clone --recurse-submodules git://gitlab.com/rainDX/gaka-cpp.git
cd gaka-cpp
mkdir build
cmake -DCMAKE_BUILD_TYPE=Release -G Ninja -B build
cmake --build build/
# lauch it
build/gaka
```

## TODO:
- [ ] Bezier curves
    - [x] From a given number of line segments
    - [ ] From a given length of line segment
    - [x] Visualization
- [ ] Bezier surfaces
    - [x] Lambertian reflectance
    - [x] Surface normals visualization
    - [ ] Parameter space
- [ ] Real-time rendering
    - [ ] BRDF Implementation
    - [ ] Texture support
    - [x] Three-point lighting
- [ ] BONUS
    - [ ] Interactive Bezier curve manipulation
    - [ ] Skybox