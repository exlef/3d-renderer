#pragma once

#include <iostream>

#include "glm/fwd.hpp"
#include "shader.hpp"

namespace ex
{
    class PostProcessing
    {
    public:
        Shader shader;
        unsigned int FBO = 0;
        unsigned int rectVAO = 0;
        unsigned int framebufferTexture = 0;

        PostProcessing(glm::vec2 framebuffer_dimensions);

    private:
        std::string m_vert_source_path = "src/ex3d/shaders/post_processing.vert";
        std::string m_frag_source_path = "src/ex3d/shaders/post_processing.frag";

    };
    
} // namespace ex
