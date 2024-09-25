# OpenGL 3D Renderer

A custom 3D renderer built with OpenGL and C++ as a personal project to explore graphics programming concepts.


<img width="797" alt="skybox_model_loading" src="https://github.com/user-attachments/assets/179c861b-43fd-41b5-a419-1b5a5439b595">


https://github.com/user-attachments/assets/4b40e94b-9d18-402a-84f6-d97dbc395640


## Features

- Modern OpenGL (3.3+) implementation
- Shader Management: Efficient creation and management of shader programs, including error handling for shader compilation and linking.
- Texture mapping and material system
- Blinn Phong model (directional and point lights)
- Shadow mapping (work in progress)
- Model loading 
- Skybox rendering
- Camera system
- Post-processing effects
- Simple entity component system
- Transformations: A robust system for handling model transformations, including translation, rotation, and scaling, using quaternions for smooth and accurate rotations.


## Future Plans

* **Complete Shadow Mapping:** Complete and optimize shadow mapping for both directional and point lights.
* **Advanced Lighting:** Introduce more advanced lighting techniques like PBR (Physically Based Rendering).
* **Physics Integration:** Add a basic physics engine to simulate real-world object interactions.
* **Game Development:** Extend this project to create simple games, building upon the renderer as the core.
* **Alpha Blending:** Implement alpha blending for transparent objects.
* **Instance Rendering:** Add support for efficient rendering of multiple instances of the same object.
* **Bloom Effect:** Implement a bloom post-processing effect for enhanced visual quality.
* **Implementing a basic animation system.**


## Dependencies

- OpenGL 3.3+
- GLEW
- GLFW
- glm
- assimp
- stb_image

## Project Structure

```
.
├── ex3d/                 # Core renderer library
├── res/                  # Resource files (models, textures)
├── shaders/              # GLSL shader files
├── main.cpp              # Application entry point
└── Makefile              # Make build configuration
```
