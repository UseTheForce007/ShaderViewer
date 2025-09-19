# Mesh Deformation Roadmap

This document outlines the next development steps for adding mesh deformation features to ShaderViewer. The goal is to enable real-time vertex manipulation and explore various deformation techniques, from simple to advanced.

## Current State
- Interactive GLSL shader editing
- 3D model loading and rendering
- Camera controls and live preview
- ImGui-based UI for shader editing

## Roadmap

1. **Basic Vertex Displacement**

- Directly manipulate vertex positions in the vertex shader.
- Experiment with uniform offsets, time-based animation, or simple mathematical functions.

2. **Applying Forces to Vertices**

- Introduce force vectors (e.g., gravity, wind, user-driven impulses).
- Update vertex positions based on applied forces, either in the shader or on the CPU.

3. **Simple Physics Simulation**

- Implement spring/mass systems or basic constraints to simulate elastic deformations.
- Explore integration methods (Euler, Verlet) for updating positions.

4. **Advanced Deformation Techniques**

- Use procedural noise, morph targets, or blend shapes for more complex effects.
- Consider mesh subdivision or dynamic topology for richer deformations.

5. **UI Controls for Deformation Parameters**

- Add ImGui sliders/buttons to control force magnitude, direction, and other parameters in real time.

6. **Visualization & Debugging**

- Display vertex normals, force vectors, or stress maps to help debug and tune deformation algorithms.

---

**Tip:** Start with simple vertex displacement in the shader, then incrementally add complexity. Use the ImGui editor to tweak parameters and observe results live.
