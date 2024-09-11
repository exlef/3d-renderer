#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "base_shader.hpp"

namespace ex
{
    class PostProcessingShader : public BaseShader
    {
    private:
        std::string m_vert_source_path = "src/ex3d/shaders/post_processing.vert";
        std::string m_frag_source_path = "src/ex3d/shaders/post_processing.frag";

    public:
        PostProcessingShader()
        {
            create_shader_program(m_vert_source_path, m_frag_source_path);
            if (m_id == 0 || m_id == GL_INVALID_INDEX)
            {
                throw std::runtime_error("Failed to create shader program");
            }
            else
                std::cout << "here" << std::endl;
        }
    };
    
} // namespace ex
