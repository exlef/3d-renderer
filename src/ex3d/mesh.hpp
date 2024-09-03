#pragma once

#include "GL/glew.h"
#include "GLFW/glfw3.h"
#include <assimp/scene.h>
#include <glm/glm.hpp>
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
        Mesh(aiMesh* mesh, const aiScene* scene);
        void Draw(unsigned int shader);

    private:
        unsigned int VAO, VBO, EBO;
        void setup_attributes();
    };

    // impl -----------------------------------------------------------------------------------

    Mesh::Mesh(aiMesh* assimp_mesh, const aiScene* scene)
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
            vertex.Normal = vec;

            if (assimp_mesh->mTextureCoords[0]) // does the mesh contain texture coordinates?
            {
                glm::vec2 vec;
                vec.x = assimp_mesh->mTextureCoords[0][i].x;
                vec.y = assimp_mesh->mTextureCoords[0][i].y;
                vertex.TexCoords = vec;
            }
            else
            {
                vertex.TexCoords = glm::vec2(0.0f, 0.0f);
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
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
        // vertex texture coords
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

        glBindVertexArray(0);
    }

    void Mesh::Draw(unsigned int shader)
    {
        /*
            this solution requires shaders to have:

            struct Material {
            sampler2D texture_diffuse1;
            sampler2D texture_diffuse2;
            .
            .
            .
            sampler2D texture_specular1;
            sampler2D texture_specular2;
            .
            .
            .
            };
            uniform Material material;
        */
        // unsigned int diffuse_counter = 1;
        // unsigned int specular_counter = 1;
        // for (unsigned int i = 0; i < textures.size(); i++)
        // {
        //     std::string number;
        //     std::string name = textures[i].type;

        //     if (name == "texture_diffuse")
        //         number = std::to_string(diffuse_counter);
        //     else if (name == "texture_specular")
        //         number = std::to_string(specular_counter);

        //     std::string uniform_name = "material." + name + number;

        //     glActiveTexture(GL_TEXTURE0 + i); // activate proper texture unit before binding
        //     glUniform1i(glGetUniformLocation(shader, uniform_name.c_str()), i);
        //     glBindTexture(GL_TEXTURE_2D, textures[i].id);

        //     diffuse_counter++;
        //     specular_counter++;
        // }
        // glActiveTexture(GL_TEXTURE0);

        // // draw mesh
        // glBindVertexArray(VAO);
        // glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        // glBindVertexArray(0);
    }
} // namespace ex

