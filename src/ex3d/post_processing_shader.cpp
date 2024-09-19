#include "post_processing_shader.hpp"


#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace ex
{
    std::string m_vert_source_path = "src/ex3d/shaders/post_processing.vert";
    std::string m_frag_source_path = "src/ex3d/shaders/post_processing.frag";

    PostProcessingShader::PostProcessingShader()
    {
        create_shader_program(m_vert_source_path, m_frag_source_path);
    }
    
} // namespace ex
