#pragma once

#include "GLFW/glfw3.h"
#include <filesystem>
#include <fstream>
#include <sstream>
#include <iostream>
#include <string>
#include <vector>

class Shader
{
private:
    u_int32_t m_id = -1;
public:
    Shader(std::string vert_file, std::string frag_file)
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
                std::cerr << "ERROR::SHADER::SHADER_PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
                m_id = -1;
            }
        }

        glDeleteShader(vert_id);
        glDeleteShader(frag_id);
    }

    ~Shader()
    {
        if (m_id != -1)
        {
            glDeleteProgram(m_id);
        }
    }

    void use()
    {
        glUseProgram(m_id);
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
                std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n"
                          << infoLog << std::endl;
            }
        }
        return shader_id;
    }
    
};
