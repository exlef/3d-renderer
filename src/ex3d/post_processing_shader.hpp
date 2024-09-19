#pragma once


#include "base_shader.hpp"

namespace ex
{
    class PostProcessingShader : public BaseShader
    {
    private:
        std::string m_vert_source_path = "src/ex3d/shaders/post_processing.vert";
        std::string m_frag_source_path = "src/ex3d/shaders/post_processing.frag";

    public:
        PostProcessingShader();
    };
    
} // namespace ex
