#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "open_gl_error_checking.hpp"

namespace ex
{

    class UnlitShader
    {
    private:
        u_int32_t m_id = 0;
        std::string m_vert_source_path = "src/ex3d/shaders/unlit.vert", m_frag_source_path = "src/ex3d/shaders/unlit.frag";

    public:
        UnlitShader()
        {
            create_shader(m_vert_source_path, m_frag_source_path);
            if (m_id == 0 || m_id == GL_INVALID_INDEX)
            {
                throw std::runtime_error("Failed to create shader program");
            }

            setup();
        }
        ~UnlitShader() = default;

        void setup()
        {
            use();

            setVec3("color", glm::vec3(1));

            // setFloat("material.shininess", 32.0f);
        }

        void use()
        {
            if (m_id == 0 || m_id == GL_INVALID_INDEX)
            {

                throw std::runtime_error("Failed to create shader program");
            }
            glc(glUseProgram(m_id));
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

    private:
        void setVec3(const std::string& name, const glm::vec3& value) const
        {
            glUniform3fv(glGetUniformLocation(m_id, name.c_str()), 1, &value[0]);
        }

        void setMat4(const std::string& name, const glm::mat4& mat) const
        {
            glUniformMatrix4fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
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
                        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                                  << infoLog << std::endl;
                        break;
                    case GL_FRAGMENT_SHADER:
                        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n"
                                  << infoLog << std::endl;
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

