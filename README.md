# BattleShipGame

A simple Battleships game implemented in C++ using SDL3 and CMake.

## Features
- Classic Battleship gameplay
- Modern C++ (C++23)
- SDL3 rendering
- AI opponent

## Getting Started

### Prerequisites
- CMake 3.24 or newer
- A C++ compiler with C++23 support (e.g., MSVC, GCC, Clang)
- SDL3 (included as a submodule/vendor directory)

### Generating Project Files with CMake

1. **Clone the repository:**
   ```sh
   git clone --recursive https://github.com/Kalerat/BattleShipGame.git
   cd BattleShipGame
   ```

2. **Create a build directory (outside of src!):**
   ```sh
   mkdir build
   cd build
   ```

3. **Generate project files (always specify the source directory as ../src):**
   - For Visual Studio (Windows):
     ```sh
     cmake -G "Visual Studio 17 2022" -A x64 ../src
     ```
   - For Ninja (cross-platform):
     ```sh
     cmake -G Ninja ../src
     ```
   - For Unix Makefiles (Linux/macOS):
     ```sh
     cmake -G "Unix Makefiles" ../src
     ```

4. **Build the project:**
   ```sh
   cmake --build .
   ```

5. **Run the game:**
   - The executable will be in the build output directory (e.g., `build/Win_x64_Debug/` on Windows).

## Notes
- SDL3 is included in the `vendor/SDL` directory and is added as a subdirectory in CMake.
- Always run CMake from the `build` directory and specify the source as `../src` to avoid cluttering the `src` folder with build files.
- You can adjust the CMake generator and build type as needed for your platform and IDE.

## License
This project is licensed under the MIT License.
