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

### Building with CMake Presets

1. **Clone the repository:**
   ```sh
   git clone --recursive https://github.com/Kalerat/BattleShipGame.git
   cd BattleShipGame
   ```

2. **Configure the project using presets:**
   
   **For Visual Studio 2022 (Recommended for Windows):**
   ```sh
   # Debug configuration
   cmake --preset Win_x64_Debug_VS2022
   
   # Release configuration
   cmake --preset Win_x64_Release_VS2022
   ```
   
   **For Ninja (Cross-platform):**
   ```sh
   # Debug configuration
   cmake --preset Win_x64_Debug_Ninja
   
   # Release configuration
   cmake --preset Win_x64_Release_Ninja
   ```

3. **Build the project:**
   ```sh
   # Visual Studio 2022 builds
   cmake --build --preset Win_x64_Debug_VS2022_Build
   cmake --build --preset Win_x64_Release_VS2022_Build
   
   # Ninja builds
   cmake --build --preset Win_x64_Debug_Ninja_Build
   cmake --build --preset Win_x64_Release_Ninja_Build
   ```

4. **Open and run with Visual Studio 2022:**
   - After configuring with the VS2022 preset, open the generated `BattleShipGame.sln` file in the `build/Win_x64_Debug_VS2022/` or `build/Win_x64_Release_VS2022/` directory.
   - Right-click the `BattleShipGame` project in the Solution Explorer and choose **Set as Startup Project**.
   - Press `F5` to build and run the game.

### Alternative: Manual CMake Commands

If you prefer not to use presets, you can still use traditional CMake commands:

1. **Create a build directory:**
   ```sh
   mkdir build
   cd build
   ```

2. **Generate project files:**
   - For Visual Studio (Windows):
     ```sh
     cmake -G "Visual Studio 17 2022" -A x64 ..
     ```
   - For Ninja (cross-platform):
     ```sh
     cmake -G Ninja ..
     ```

3. **Build the project:**
   ```sh
   cmake --build .
   ```

6. **Run the game:**
   - The executable will be in the build output directory:
     - Visual Studio builds: `build/Win_x64_Debug_VS2022/src/Debug/` or `build/Win_x64_Release_VS2022/src/Release/`
     - Ninja builds: `build/Win_x64_Debug_Ninja/src/` or `build/Win_x64_Release_Ninja/src/`

## Available CMake Presets

The project includes the following CMake presets for easy configuration and building:

### Configure Presets:
- `Win_x64_Debug_VS2022` - Debug build for Visual Studio 2022
- `Win_x64_Release_VS2022` - Release build for Visual Studio 2022
- `Win_x64_Debug_Ninja` - Debug build with Ninja generator
- `Win_x64_Release_Ninja` - Release build with Ninja generator

### Build Presets:
- `Win_x64_Debug_VS2022_Build` - Build Debug configuration (VS2022)
- `Win_x64_Release_VS2022_Build` - Build Release configuration (VS2022)
- `Win_x64_Debug_Ninja_Build` - Build Debug configuration (Ninja)
- `Win_x64_Release_Ninja_Build` - Build Release configuration (Ninja)

## Notes
- CMake presets automatically create separate build directories for each configuration to avoid conflicts.
- SDL3 is included in the `vendor/SDL` directory and is added as a subdirectory in CMake.
- The presets are configured to work from the project root directory (no need to create or navigate to a build folder manually).
- Visual Studio 2022 presets generate `.sln` files that can be opened directly in the IDE.

## License
This project is licensed under the MIT License.
