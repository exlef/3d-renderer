#pragma once 

#include "base_shader.hpp"

namespace ex
{
    class ShadowMapShader : public BaseShader 
    {
    private:
        std::string m_vert_source_path = "src/ex3d/shaders/shadow_map.vert";
        std::string m_frag_source_path = "src/ex3d/shaders/shadow_map.frag";

    public:
        ShadowMapShader()
        {
            create_shader_program(m_vert_source_path, m_frag_source_path);
            if (m_id == 0 || m_id == GL_INVALID_INDEX)
            {
                throw std::runtime_error("Failed to create shader program");
            }

            
        }
        ~ShadowMapShader() = default;
    };

} // namespace ex
