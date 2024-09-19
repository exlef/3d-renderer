#pragma once


#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <string>

#include "open_gl_error_checking.hpp"

namespace ex
{
    class BaseShader
    {
    public:
        BaseShader() = default;
        virtual ~BaseShader() = default;
        // Copy constructor
        BaseShader(const BaseShader& other) = delete;
        // Copy Assignment Operator
        BaseShader& operator=(const BaseShader& other) = delete;
        // Move Constructor (deleted)
        BaseShader(BaseShader&& other) = delete;
        // Move Assignment Operator (deleted)
        BaseShader& operator=(BaseShader&& other) = delete;
        
        void use();
        virtual void update() { }
        inline u_int32_t id() const { return m_id; }

        // utility uniform functions
        // ------------------------------------------------------------------------
        void setTexture(const std::string& name, int id);
        void setBool(const std::string& name, bool value) const;
        void setInt(const std::string& name, int value) const;
        void setFloat(const std::string& name, float value) const;
        void setVec2(const std::string& name, const glm::vec2& value) const;
        void setVec2(const std::string& name, float x, float y) const;
        void setVec3(const std::string& name, const glm::vec3& value) const;
        void setVec3(const std::string& name, float x, float y, float z) const;
        void setVec4(const std::string& name, const glm::vec4& value) const;
        void setVec4(const std::string& name, float x, float y, float z, float w) const;
        void setMat2(const std::string& name, const glm::mat2& mat) const;
        void setMat3(const std::string& name, const glm::mat3& mat) const;
        void setMat4(const std::string& name, const glm::mat4& mat) const;
    
    protected:
        u_int32_t m_id = 0;

        void create_shader_program(const std::string& vert_file, const std::string& frag_file);
    
    };
} // namespace ex
