#pragma once

#include "base_shader.hpp"
#include "camera.hpp"

namespace ex
{
    class SkyboxShader : BaseShader
    {
    private:
        std::string m_vert_source_path = "src/ex3d/shaders/skybox.vert";
        std::string m_frag_source_path = "src/ex3d/shaders/skybox.frag";

        /* data */
    public:
        SkyboxShader() = default;
        void init()
        {
            create_shader_program(m_vert_source_path, m_frag_source_path);
            if (m_id == 0 || m_id == GL_INVALID_INDEX)
            {
                throw std::runtime_error("Failed to create shader program");
            }
        }

        void update(Camera* cam)
        {
            use();
            setTexture("skybox", 0);
            // We make the mat4 into a mat3 and then a mat4 again in order to get rid of the last row and column
            // The last row and column affect the translation of the skybox (which we don't want to affect)
            glm::mat4 view = glm::mat4(glm::mat3(cam->get_view_matrix()));
            // TODO: find what's wrong with this
            glm::mat4 projection = cam->get_projection_matrix();
            view = glm::mat4(1);
            projection = glm::mat4(1);
            setMat4("view", view);
            setMat4("projection", projection);
        }
    };
} // namespace ex
