#pragma once

#include <GLFW/glfw3.h>

#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>

#include "camera.hpp"
#include "light.hpp"
#include "texture.hpp"
#include "light.hpp"

namespace ex
{

    class Shader
    {
    private:
        u_int32_t m_id = 0;
        std::string m_vert_source_path = "src/shaders/default.vert", m_frag_source_path = "src/shaders/default.frag";
        // https://stackoverflow.com/questions/7322147/what-is-the-range-of-opengl-texture-id
        uint32_t m_diffuse_texture_id = 0, m_spec_texture_id = 0;
        glm::vec3 m_sky_light;

    public:
        u_int32_t id() const { return m_id; }
        Shader() = delete;
        Shader(uint32_t diffuse_texture_id = 0, uint32_t spec_texture_id = 0, glm::vec3 sky_light = glm::vec3(0.2f))
        {
            create_shader(m_vert_source_path, m_frag_source_path);
            m_diffuse_texture_id = diffuse_texture_id;
            m_spec_texture_id = spec_texture_id;
            m_sky_light = sky_light;
            setup();
        }

        ~Shader()
        {
            if (m_id != 0)
            {
                glDeleteProgram(m_id);
            }
        }

        void create_shader(const std::string& vert_file, const std::string& frag_file)
        {
            m_id = glCreateProgram();

            uint32_t vert_id = create_shader(GL_VERTEX_SHADER, vert_file);
            uint32_t frag_id = create_shader(GL_FRAGMENT_SHADER, frag_file);

            glAttachShader(m_id, vert_id);
            glAttachShader(m_id, frag_id);

            glLinkProgram(m_id);

            { // error handling code
                int success;
                char infoLog[512];
                glGetProgramiv(m_id, GL_LINK_STATUS, &success);
                if (!success)
                {
                    glGetProgramInfoLog(m_id, 512, NULL, infoLog);
                    std::cerr << "ERROR::SHADER::SHADER_PROGRAM::LINKING_FAILED\n"
                              << infoLog << std::endl;
                    m_id = -1;
                }
            }

            glDeleteShader(vert_id);
            glDeleteShader(frag_id);
        }

        void setup()
        {
            use();

            // set a default color for diffuse in case there is no textures provided
            if (m_diffuse_texture_id == 0)
            {
                ex::Texture m_default_texture = ex::Texture("src/ex3d/res/default2.png");
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, m_default_texture.id());
                glUniform1i(glGetUniformLocation(m_id, "material.diffuse"), 0);
            }
            else
            {
                glActiveTexture(GL_TEXTURE0);
                glBindTexture(GL_TEXTURE_2D, m_diffuse_texture_id);
                glUniform1i(glGetUniformLocation(m_id, "material.diffuse"), 0);
            }

            if (m_spec_texture_id != 0)
            {
                glActiveTexture(GL_TEXTURE1);
                glBindTexture(GL_TEXTURE_2D, m_spec_texture_id);
                glUniform1i(glGetUniformLocation(m_id, "material.specular"), 1);
            }

            setVec3("skyLight", m_sky_light);

            setFloat("material.shininess", 32.0f);

            // reset the openGL state
            glUseProgram(0);
        }
        
        void use()
        {
            glUseProgram(m_id);
        }

        void set_model_matrix(const glm::mat4& mat)
        {
            setMat4("model", mat);
        }

        void set_view_matrix(const glm::mat4& mat)
        {
            setMat4("view", mat);
        }

        void set_projection_matrix(const glm::mat4& mat)
        {
            setMat4("projection", mat);
        }

        void set_color(const glm::vec3& color)
        {
            setVec3("material.color", color);
        }

        void set_textures(uint32_t texture_id)
        {
            glActiveTexture(GL_TEXTURE0);
            glBindTexture(GL_TEXTURE_2D, texture_id);
        }

        void set_view_pos(glm::vec3 cam_pos)
        {
            setVec3("viewPos", cam_pos);
        }

        void set_directional_light(DirectionalLight dir_light)
        {
            setVec3("dirLight.direction", dir_light.dir);
            setVec3("dirLight.color", dir_light.color);
        }

        // utility uniform functions
        // ------------------------------------------------------------------------
        void setBool(const std::string& name, bool value) const
        {
            glUniform1i(glGetUniformLocation(m_id, name.c_str()), (int)value);
        }
        // ------------------------------------------------------------------------
        void setInt(const std::string& name, int value) const
        {
            glUniform1i(glGetUniformLocation(m_id, name.c_str()), value);
        }
        // ------------------------------------------------------------------------
        void setFloat(const std::string& name, float value) const
        {
            glUniform1f(glGetUniformLocation(m_id, name.c_str()), value);
        }
        // ------------------------------------------------------------------------
        void setVec2(const std::string& name, const glm::vec2& value) const
        {
            glUniform2fv(glGetUniformLocation(m_id, name.c_str()), 1, &value[0]);
        }
        void setVec2(const std::string& name, float x, float y) const
        {
            glUniform2f(glGetUniformLocation(m_id, name.c_str()), x, y);
        }
        // ------------------------------------------------------------------------
        void setVec3(const std::string& name, const glm::vec3& value) const
        {
            glUniform3fv(glGetUniformLocation(m_id, name.c_str()), 1, &value[0]);
        }
        void setVec3(const std::string& name, float x, float y, float z) const
        {
            glUniform3f(glGetUniformLocation(m_id, name.c_str()), x, y, z);
        }
        // ------------------------------------------------------------------------
        void setVec4(const std::string& name, const glm::vec4& value) const
        {
            glUniform4fv(glGetUniformLocation(m_id, name.c_str()), 1, &value[0]);
        }
        void setVec4(const std::string& name, float x, float y, float z, float w) const
        {
            glUniform4f(glGetUniformLocation(m_id, name.c_str()), x, y, z, w);
        }
        // ------------------------------------------------------------------------
        void setMat2(const std::string& name, const glm::mat2& mat) const
        {
            glUniformMatrix2fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
        }
        // ------------------------------------------------------------------------
        void setMat3(const std::string& name, const glm::mat3& mat) const
        {
            glUniformMatrix3fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
        }
        // ------------------------------------------------------------------------
        void setMat4(const std::string& name, const glm::mat4& mat) const
        {
            glUniformMatrix4fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
        }

    private:
        std::string load_shader_source(const std::string& relativePath)
        {
            // Get the current working directory
            std::filesystem::path currentPath = std::filesystem::current_path();

            // Construct the full path
            std::filesystem::path fullPath = currentPath / relativePath;

            // Ensure the file exists
            if (!std::filesystem::exists(fullPath))
            {
                throw std::runtime_error("ERROR::SHADER::FILE_NOT_FOUND: " + fullPath.string());
            }

            std::string shaderCode;
            std::ifstream shaderFile;

            // Ensure ifstream objects can throw exceptions
            shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

            try
            {
                // Open file
                shaderFile.open(fullPath);
                std::stringstream shaderStream;

                // Read file's buffer contents into stream
                shaderStream << shaderFile.rdbuf();

                // Close file handler
                shaderFile.close();

                // Convert stream into string
                shaderCode = shaderStream.str();
            }
            catch (std::ifstream::failure& e)
            {
                throw std::runtime_error("ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ: " + fullPath.string());
            }

            return shaderCode;
        }

        unsigned int create_shader(GLenum shader_type, std::string relative_path_to_shader_source)
        {
            unsigned int shader_id = glCreateShader(shader_type);
            std::string shader_source = load_shader_source(relative_path_to_shader_source);
            const char* source_ptr = shader_source.c_str();

            glShaderSource(shader_id, 1, &source_ptr, NULL);
            glCompileShader(shader_id);

            { // error handling code
                int success;
                char infoLog[512];
                glGetShaderiv(shader_id, GL_COMPILE_STATUS, &success);

                if (!success)
                {
                    glGetShaderInfoLog(shader_id, 512, NULL, infoLog);
                    switch (shader_type)
                    {
                    case GL_VERTEX_SHADER:
                        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
                        break;
                    case GL_FRAGMENT_SHADER:
                        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n" << infoLog << std::endl;
                        break;
                    default:
                        break;
                    }
                }
            }
            return shader_id;
        }
    };
} // namespace ex

