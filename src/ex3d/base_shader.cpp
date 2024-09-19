#include "base_shader.hpp"

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <filesystem>
#include <fstream>
#include <sstream>

namespace ex 
{
    std::string load_shader_source(const std::string& relativePath);
    unsigned int create_shader(GLenum shader_type, std::string relative_path_to_shader_source);

    void BaseShader::create_shader_program(const std::string& vert_file, const std::string& frag_file)
    {
        glc(m_id = glCreateProgram());

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

        if (m_id == 0 || m_id == GL_INVALID_INDEX)
        {
            throw std::runtime_error("Failed to create shader program");
        }

        glDeleteShader(vert_id);
        glDeleteShader(frag_id);
    }

     void BaseShader::use()
    {
        if (m_id == 0 || m_id == GL_INVALID_INDEX)
        {

            throw std::runtime_error("Failed to create shader program");
        }
        glc(glUseProgram(m_id));
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

     // utility uniform functions
    // ------------------------------------------------------------------------
    void BaseShader::setTexture(const std::string& name, int id)
    {
        glUniform1i(glGetUniformLocation(m_id, name.c_str()), id);
    }
    void BaseShader::setBool(const std::string& name, bool value) const
    {
        glUniform1i(glGetUniformLocation(m_id, name.c_str()), (int)value);
    }
    void BaseShader::setInt(const std::string& name, int value) const
    {
        glUniform1i(glGetUniformLocation(m_id, name.c_str()), value);
    }
    void BaseShader::setFloat(const std::string& name, float value) const
    {
        glUniform1f(glGetUniformLocation(m_id, name.c_str()), value);
    }
    void BaseShader::setVec2(const std::string& name, const glm::vec2& value) const
    {
        glUniform2fv(glGetUniformLocation(m_id, name.c_str()), 1, &value[0]);
    }
    void BaseShader::setVec2(const std::string& name, float x, float y) const
    {
        glUniform2f(glGetUniformLocation(m_id, name.c_str()), x, y);
    }
    void BaseShader::setVec3(const std::string& name, const glm::vec3& value) const
    {
        glUniform3fv(glGetUniformLocation(m_id, name.c_str()), 1, &value[0]);
    }
    void BaseShader::setVec3(const std::string& name, float x, float y, float z) const
    {
        glUniform3f(glGetUniformLocation(m_id, name.c_str()), x, y, z);
    }
    void BaseShader::setVec4(const std::string& name, const glm::vec4& value) const
    {
        glUniform4fv(glGetUniformLocation(m_id, name.c_str()), 1, &value[0]);
    }
    void BaseShader::setVec4(const std::string& name, float x, float y, float z, float w) const
    {
        glUniform4f(glGetUniformLocation(m_id, name.c_str()), x, y, z, w);
    }
    void BaseShader::setMat2(const std::string& name, const glm::mat2& mat) const
    {
        glUniformMatrix2fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    void BaseShader::setMat3(const std::string& name, const glm::mat3& mat) const
    {
        glUniformMatrix3fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
    void BaseShader::setMat4(const std::string& name, const glm::mat4& mat) const
    {
        glUniformMatrix4fv(glGetUniformLocation(m_id, name.c_str()), 1, GL_FALSE, &mat[0][0]);
    }
}