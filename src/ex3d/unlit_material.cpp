#include "unlit_material.hpp"

namespace ex 
{
    UnlitShader::UnlitShader(const Camera* cam)
    {
        shader.create_shader_program(m_vert_source_path, m_frag_source_path);

        m_cam = cam;

        shader.use();
        shader.setVec3("color", color);
    }

    void UnlitShader::update() 
    { 
        // use();

        // setMat4("model", model_mat);
        // setMat4("view", view_mat);
        // setMat4("projection", proj_mat);
        // setVec3("color", color);
    }
}