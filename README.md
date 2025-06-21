# Shader Viewer

A live GLSL shader development environment that lets you:
- Write GLSL shaders and see instant updates
- Preview shaders on a 3D model
- Use shaders that are directly compatible with OpenGL engines

## Controls

- WASD: Move camera
- Mouse: Look around
- Mouse wheel: Zoom
- ESC: Exit

## Usage

1. Create/edit shaders in the `shaders` folder
2. Shaders are automatically reloaded when saved
3. See changes instantly in the viewer

## Project Structure

- `shaders/`: GLSL shader files
  - `default.vert`: Vertex shader
  - `default.frag`: Fragment shader
- `assets/`: 3D models and textures
- `src/`: Source code
  - `main.cpp`: Application entry point
  - `shader.cpp`: Shader loading and hot-reloading
  - `model.cpp`: 3D model loading
  - `camera.cpp`: Camera controls

## Building

1. Make sure you have CMake installed
2. Create a build directory: `cmake -S . -B build`
3. Build the project: `cmake --build build`
4. Run the executable in the build folder

## Dependencies

- GLFW: Window creation and input handling
- GLAD: OpenGL function loading
- GLM: Math library
- tinyobjloader: OBJ file loading
