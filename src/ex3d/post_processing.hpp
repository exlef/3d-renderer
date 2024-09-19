#pragma once

#include <iostream>

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

        PostProcessing(int width, int height);
    };
    
} // namespace ex
