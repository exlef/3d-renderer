#pragma once

#include <iostream>

#include "glm/fwd.hpp"
#include "shader.hpp"

namespace ex
{
    class PostProcessing
    {
    public:
        PostProcessing(glm::vec2 framebuffer_dimensions);
        void prepare();
        void draw();

    private:
        std::string m_vert_source_path = "src/ex3d/shaders/post_processing.vert";
        std::string m_frag_source_path = "src/ex3d/shaders/post_processing.frag";
        Shader m_shader;
        unsigned int m_FBO = 0;
        unsigned int m_rectVAO = 0;
        unsigned int m_framebufferTexture = 0;

    };
    
} // namespace ex
