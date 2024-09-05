#pragma once

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <assimp/scene.h>
#include <glm/glm.hpp>
#include <vector>
#include <iostream>

namespace ex
{

    struct Vertex
    {
        glm::vec3 Position;
        // glm::vec3 Normal;
        // glm::vec2 TexCoords;
    };

    class Mesh
    {
    public:
        // mesh data
        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;

        Mesh() = delete;
        Mesh(aiMesh* mesh /*, const aiScene* scene*/);
        void Draw();

    private:
        unsigned int VAO, VBO, EBO;
        void setup_attributes();
    };

    // impl -----------------------------------------------------------------------------------

    Mesh::Mesh(aiMesh* assimp_mesh /*, const aiScene* scene*/)
    {
        // process vertex positions, normals and texture coordinates
        for (unsigned int i = 0; i < assimp_mesh->mNumVertices; i++)
        {
            Vertex vertex;
            glm::vec3 vec;
            vec.x = assimp_mesh->mVertices[i].x;
            vec.y = assimp_mesh->mVertices[i].y;
            vec.z = assimp_mesh->mVertices[i].z;
            vertex.Position = vec;

            vec.x = assimp_mesh->mNormals[i].x;
            vec.y = assimp_mesh->mNormals[i].y;
            vec.z = assimp_mesh->mNormals[i].z;
            // vertex.Normal = vec;

            if (assimp_mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
            {
                glm::vec2 vec;
                vec.x = assimp_mesh->mTextureCoords[0][i].x;
                vec.y = assimp_mesh->mTextureCoords[0][i].y;
                // vertex.TexCoords = vec;
            }
            else
            {
                // vertex.TexCoords = glm::vec2(0.0f, 0.0f);
            }

            vertices.push_back(vertex);
        }
        // process indices
        for (unsigned int i = 0; i < assimp_mesh->mNumFaces; i++)
        {
            aiFace face = assimp_mesh->mFaces[i];

            for (unsigned int j = 0; j < face.mNumIndices; j++)
            {
                indices.push_back(face.mIndices[j]);
            }
        }

        std::cout << indices.size() << std::endl;

        setup_attributes();
    }

    void Mesh::setup_attributes()
    {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), &indices[0], GL_STATIC_DRAW);

        // vertex positions
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
        // vertex normals
        // glEnableVertexAttribArray(1);
        // glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
        // vertex texture coords
        // glEnableVertexAttribArray(2);
        // glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

        glBindVertexArray(0);
    }

    void Mesh::Draw()
    {
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }
} // namespace ex

/*

*/