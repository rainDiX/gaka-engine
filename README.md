# Gaka-cpp (画家)

An OpenGL 4.3+ Renderer (Vulkan WIP in another branch), written in C++23

![Legacy Gaka rendering a Bezier surface in a SDL2 Window](/.readme/bezier_surface.png?raw=true "Legacy Gaka rendering a Bezier surface")

## Build instructions
Build dependencies :
- CMake
- a C++23 compiler

Build Dependencies :
- Required on system :
    - For SDL2: libx11-dev libxft-dev libxext-dev libwayland-dev libxkbcommon-dev libegl1-mesa-dev libgles2-mesa-dev
- Bundled via vcpkg (optional):
    - SDL2
    - libepoxy (OpenGL Function Loader)
    - Sail (image loading)
    - assimp (model loading)


### VCPKG instructions
Installing dependencies on Fedora:
```sh
dnf install libX11-devel libXrender-devel libXext-devel libXcuror-devel libxkbcommon-devel wayland-devel libglvnd-devel mesa-libGLU-devel mesa-libEGL-devel
```
Installing dependencies on Ubuntu:
```sh
apt install apt install libx11-dev libxft-dev libxext-dev libxkbcommon-dev libwayland-dev libglvnd-dev libgles2-mesa-dev libegl1-mesa-dev
```

Building
```sh
git clone --recurse-submodules git://gitlab.com/rainDX/gaka-engine.git
cd gaka-engine
mkdir build
cmake -G Ninja -B build -DCMAKE_TOOLCHAIN_FILE=dev/vcpkg/scripts/buildsystems/vcpkg.cmake
cmake --build build --parallel
# lauch it
build/src/gaka_gl
```

**Notes:** If needed, some CMake presets are provided in `CMakePresets.json`


## Programming Guide
- uses C++23
- No C++ Modules
- C++20's `char8_t` has been disabled
- No exceptions (use std::expected instead)
- Format style based on Google (see clang-format)
