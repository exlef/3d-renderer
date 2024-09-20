#pragma once

#include <iostream>

#include "glm/fwd.hpp"
#include "post_processing_shader.hpp"

namespace ex
{
    class PostProcessing
    {
    public:
        unsigned int FBO = 0;
        unsigned int rectVAO = 0;
        unsigned int framebufferTexture = 0;
        PostProcessingShader pp_shader_prog;

        PostProcessing(glm::vec2 framebuffer_dimensions);
    };
    
} // namespace ex
