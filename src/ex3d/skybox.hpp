#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
// #include <stb_image.h>

#include <filesystem>
#include <vector>
#include <string>
#include <iostream>

#include "skybox_shader.hpp"
#include "open_gl_error_checking.hpp"
#include "camera.hpp"

namespace ex
{
    class Skybox
    {
    private:
        SkyboxShader m_skybox_shader;

        // loads a cubemap texture from 6 individual texture faces
        // order:
        // +X (right)
        // -X (left)
        // +Y (top)
        // -Y (bottom)
        // +Z (front)
        // -Z (back)
        // -------------------------------------------------------
        unsigned int loadCubemap(std::vector<std::string> faces)
        {
            // openGL handle cubemap differently so it shouldn't be flipped like textures
            stbi_set_flip_vertically_on_load(false);

            // TODO: make glc also check if OpenGL is created because every opengl call will seg fault without sending any error if OpenGL is not initialized
            // Check if the OpenGL context is created
            if (glfwGetCurrentContext() == nullptr)
            {
                std::cout << "OpenGL context is not created." << std::endl;
            }

            unsigned int textureID;
            glc(glGenTextures(1, &textureID));
            glc(glBindTexture(GL_TEXTURE_CUBE_MAP, textureID));


            int width, height, nrChannels;
            for (unsigned int i = 0; i < faces.size(); i++)
            {
                unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
                if (data)
                {
                    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
                    stbi_image_free(data);
                }
                else
                {
                    std::cout << "Cubemap texture failed to load at path: " << faces[i] << std::endl;
                    stbi_image_free(data);
                }
            }
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

            // reset states back since we need it this way for textures
            stbi_set_flip_vertically_on_load(true); // Flip the image vertically

            return textureID;
        }

    public:
        unsigned int skyboxVAO;
        unsigned int cubemapTexture;
        

        // void update_shader(Camera* cam)
        // {
        //     skybox_shader.update(cam);
        // }

        Skybox()
        {
            std::vector<std::string> faces{
                std::filesystem::path("src/res/skybox/right.jpg"),
                std::filesystem::path("src/res/skybox/left.jpg"),
                std::filesystem::path("src/res/skybox/top.jpg"),
                std::filesystem::path("src/res/skybox/bottom.jpg"),
                std::filesystem::path("src/res/skybox/front.jpg"),
                std::filesystem::path("src/res/skybox/back.jpg")};

            cubemapTexture = loadCubemap(faces);

            float skyboxVertices[] = {
                // positions
                -1.0f, 1.0f, -1.0f,
                -1.0f, -1.0f, -1.0f,
                1.0f, -1.0f, -1.0f,
                1.0f, -1.0f, -1.0f,
                1.0f, 1.0f, -1.0f,
                -1.0f, 1.0f, -1.0f,

                -1.0f, -1.0f, 1.0f,
                -1.0f, -1.0f, -1.0f,
                -1.0f, 1.0f, -1.0f,
                -1.0f, 1.0f, -1.0f,
                -1.0f, 1.0f, 1.0f,
                -1.0f, -1.0f, 1.0f,

                1.0f, -1.0f, -1.0f,
                1.0f, -1.0f, 1.0f,
                1.0f, 1.0f, 1.0f,
                1.0f, 1.0f, 1.0f,
                1.0f, 1.0f, -1.0f,
                1.0f, -1.0f, -1.0f,

                -1.0f, -1.0f, 1.0f,
                -1.0f, 1.0f, 1.0f,
                1.0f, 1.0f, 1.0f,
                1.0f, 1.0f, 1.0f,
                1.0f, -1.0f, 1.0f,
                -1.0f, -1.0f, 1.0f,

                -1.0f, 1.0f, -1.0f,
                1.0f, 1.0f, -1.0f,
                1.0f, 1.0f, 1.0f,
                1.0f, 1.0f, 1.0f,
                -1.0f, 1.0f, 1.0f,
                -1.0f, 1.0f, -1.0f,

                -1.0f, -1.0f, -1.0f,
                -1.0f, -1.0f, 1.0f,
                1.0f, -1.0f, -1.0f,
                1.0f, -1.0f, -1.0f,
                -1.0f, -1.0f, 1.0f,
                1.0f, -1.0f, 1.0f};

            unsigned int skyboxVBO;
            glc(glGenVertexArrays(1, &skyboxVAO));
            glc(glGenBuffers(1, &skyboxVBO));
            glc(glBindVertexArray(skyboxVAO));
            glc(glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO));
            glc(glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW));
            glc(glEnableVertexAttribArray(0));
            glc(glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0));
        }

        void update_shader(const Camera* cam)
        {
            m_skybox_shader.update_skybox(cam);
        }

        // Skybox() = default;
        ~Skybox() = default;
    };
    
} // namespace ex
