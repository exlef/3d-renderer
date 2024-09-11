#pragma once

#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>

#include <string>

namespace ex
{
    class BaseShader
    {
    private:
        /* data */
    protected:
        u_int32_t m_id = 0;
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
    };
} // namespace ex
