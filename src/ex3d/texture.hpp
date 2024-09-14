// #pragma once

// #include <string>
// #define STB_IMAGE_IMPLEMENTATION
// #include <stb_image.h>
// #include <GL/glew.h>
// #include <GLFW/glfw3.h>
// #include <iostream>

// namespace ex
// {
//     class Texture
//     {
//     private:
//         uint32_t m_textureID;
//     public:
//         uint32_t id() const { return m_textureID; }

//         Texture(const std::string& path);
//         ~Texture();
//     };

//     Texture::Texture(const std::string& path)
//     {
//         unsigned int textureID;
//         glGenTextures(1, &textureID);

//         int width, height, nrComponents;
//         unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);
//         if (data)
//         {
//             GLenum format = 1;
//             if (nrComponents == 1)
//                 format = GL_RED;
//             else if (nrComponents == 3)
//                 format = GL_RGB;
//             else if (nrComponents == 4)
//                 format = GL_RGBA;

//             glBindTexture(GL_TEXTURE_2D, textureID);
//             glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
//             glGenerateMipmap(GL_TEXTURE_2D);

//             /*
//                 Note that when sampling textures at their borders, OpenGL interpolates the border values with the next repeated value of the texture (because we set its wrapping parameters to GL_REPEAT by default). This is usually okay, but since we're using transparent values, the top of the texture image gets its transparent value interpolated with the bottom border's solid color value. The result is then a slightly semi-transparent colored border you may see wrapped around your textured quad. To prevent this, set the texture wrapping method to GL_CLAMP_TO_EDGE whenever you use alpha textures that you don't want to repeat:
//             */
//             if (format == GL_RGBA)
//             {
//                 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//                 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
//             }
//             else
//             {
//                 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//                 glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//             }
//             glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
//             glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

//             stbi_image_free(data);
//         }
//         else
//         {
//             std::cout << "Texture failed to load at path: " << path << std::endl;
//             stbi_image_free(data);
//         }
        
//         // unbind the texture
//         // glBindTexture(GL_TEXTURE_2D, 0);

//         m_textureID = textureID;
//     }
    
//     Texture::~Texture()
//     {
//     }
    
// } // namespace ex

#pragma once
#include <string>
// #define STB_IMAGE_IMPLEMENTATION
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <iostream>
// #include <stb_image.h>

namespace ex
{
    class Texture
    {
    private:
        uint32_t m_textureID;
        bool m_is_srgb = APPLY_GAMMA_CRRC;

    public:
        uint32_t id() const { return m_textureID; }
        Texture(const std::string& path, bool is_srgb = APPLY_GAMMA_CRRC);
        ~Texture();
    };

    Texture::Texture(const std::string& path, bool is_srgb)
        : m_is_srgb(is_srgb)
    {
        glGenTextures(1, &m_textureID);

        int width, height, nrComponents;
        stbi_set_flip_vertically_on_load(true); // Flip the image vertically
        unsigned char* data = stbi_load(path.c_str(), &width, &height, &nrComponents, 0);

        if (data)
        {
            GLenum internalFormat = GL_RGB;
            GLenum dataFormat = GL_SRGB;
            if (nrComponents == 1)
            {
                internalFormat = dataFormat = GL_RED;
            }
            else if (nrComponents == 3)
            {
                internalFormat = m_is_srgb ? GL_SRGB : GL_RGB;
                dataFormat = GL_RGB;
            }
            else if (nrComponents == 4)
            {
                internalFormat = m_is_srgb ? GL_SRGB_ALPHA : GL_RGBA;
                dataFormat = GL_RGBA;
            }

            glBindTexture(GL_TEXTURE_2D, m_textureID);
            glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);

            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, dataFormat == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, dataFormat == GL_RGBA ? GL_CLAMP_TO_EDGE : GL_REPEAT);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            stbi_image_free(data);
        }
        else
        {
            std::cout << "Texture failed to load at path: " << path << std::endl;
            stbi_image_free(data);
        }

        glBindTexture(GL_TEXTURE_2D, 0); // Unbind the texture
    }

    Texture::~Texture()
    {
        glDeleteTextures(1, &m_textureID);
    }

} // namespace ex