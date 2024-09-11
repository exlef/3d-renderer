#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include <filesystem>
#include <fstream>
#include <sstream>
#include <string>

#include "open_gl_error_checking.hpp"


namespace ex
{
    class BaseShader
    {
    private:
        /* data */
    protected:
        u_int32_t m_id = 0;

        void create_shader_program(const std::string& vert_file, const std::string& frag_file)
        {
            std::cout << "here" << std::endl;
            m_id = glCreateProgram();
            std::cout << m_id << std::endl;
            std::cout << "here2" << std::endl;

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

    public:
        BaseShader() = default;

        virtual ~BaseShader()
        {
            // I want to be aware how many shaders are being destroyed
            // to not mistakenly destroy them
            // becuase if I copy a shader (for example when pass by value to a function)
            // and then this copy being destroyed. it will destroy the original one too because they
            // are obviously shares the same id. so we are telling openGL destroy the shader with this id and
            // the original one and the copy one both are not valid.
            // I need to be careful about this.
            static int shader_count = 1;
            std::cout << std::endl;
            ;
            std::cout << "[shader destroyed] id: " << m_id << std::endl;
            std::cout << "total destroyed shader count is: " << shader_count << std::endl;
            std::cout << std::endl;
            ;
            shader_count++;
            if (m_id != 0)
            {
                glDeleteProgram(m_id);
            }
        }

        // I want to know when shaders being copied since they will the share the same id and when destroyed this id will be used to delete shader from the memory so both shaders are going to be invalid.
        // TODO: what we need to do instead of throwing error?
        // Copy constructor
        BaseShader(const BaseShader& other)
        {
            UNUSED(other);
            std::cout << RED << "error" << RESET << std::endl;
            throw std::runtime_error("Copy constructor for Shader called\n");
        }

        // Copy Assignment Operator
        BaseShader& operator=(const BaseShader& other)
        {
            UNUSED(other);
            std::cout << RED << "error" << RESET << std::endl;
            throw std::runtime_error("Copy assignment operator for Shader called\n");
        }

        // Move Constructor (deleted)
        BaseShader(BaseShader&& other) = delete;

        // Move Assignment Operator (deleted)
        BaseShader& operator=(BaseShader&& other) = delete;

        // ------------------------------------------------------------------------
        void use()
        {
            if (m_id == 0 || m_id == GL_INVALID_INDEX)
            {

                throw std::runtime_error("Failed to create shader program");
            }
            glc(glUseProgram(m_id));
        }
        // utility uniform functions
        // ------------------------------------------------------------------------
        void setTexture(const std::string& name, int id)
        {
            glUniform1i(glGetUniformLocation(m_id, name.c_str()), id);
        }
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
    };
} // namespace ex
