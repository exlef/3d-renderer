#pragma once


#include <glm/glm.hpp>
#include <assimp/scene.h>

#include <vector>

namespace ex
{
    struct Vertex
    {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoords;
    };

    class Mesh
    {
    public:
        // mesh data
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        Mesh() = delete;
        Mesh(aiMesh* assimp_mesh);
        void Draw() const;

    private:
        unsigned int VAO, VBO, EBO;
        void setup_attributes();
    };
} // namespace ex
