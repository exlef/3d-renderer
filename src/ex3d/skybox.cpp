#include "skybox.hpp"


#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <filesystem>
#include <vector>
#include <string>
#include <iostream>

#include "open_gl_error_checking.hpp"
#include "stb_image.hpp"

namespace ex
{
    Skybox::Skybox()
    {
        std::vector<std::string> faces{
            std::filesystem::path("src/res/skybox2/right.jpg"),
            std::filesystem::path("src/res/skybox2/left.jpg"),
            std::filesystem::path("src/res/skybox2/top.jpg"),
            std::filesystem::path("src/res/skybox2/bottom.jpg"),
            std::filesystem::path("src/res/skybox2/front.jpg"),
            std::filesystem::path("src/res/skybox2/back.jpg")};

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

        shader.create_shader_program(m_vert_source_path, m_frag_source_path);
    }

    void Skybox::draw(const Camera *cam)
    {
        // update skybox material
        shader.use();
        shader.setTexture("skybox", 0);
        // We make the mat4 into a mat3 and then a mat4 again in order to get rid of the last row and column
        // The last row and column affect the translation of the skybox (which we don't want to affect)
        glm::mat4 view = glm::mat4(glm::mat3(cam->get_view_matrix()));
        glm::mat4 projection = cam->get_projection_matrix();
        shader.setMat4("view", view);
        shader.setMat4("projection", projection);

        // draw skybox (as farthest object)
        glc(glDepthFunc(GL_LEQUAL)); // change depth function so depth test passes when values are equal to depth buffer's content
        glc(glBindVertexArray(skyboxVAO));
        glc(glActiveTexture(GL_TEXTURE0));
        glc(glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture));
        glc(glDrawArrays(GL_TRIANGLES, 0, 36));
        glc(glBindVertexArray(0));
        glc(glDepthFunc(GL_LESS)); // set depth function back to default
    }

    // loads a cubemap texture from 6 individual texture faces
    // order:
    // +X (right)
    // -X (left)
    // +Y (top)
    // -Y (bottom)
    // +Z (front)
    // -Z (back)
    // -------------------------------------------------------
    unsigned int Skybox::loadCubemap(std::vector<std::string> faces)
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

} // namespace ex
