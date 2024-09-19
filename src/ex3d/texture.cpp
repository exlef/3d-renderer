#include "texture.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "stb_image.hpp"

namespace ex
{
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