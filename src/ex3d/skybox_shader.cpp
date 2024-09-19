#include "skybox_shader.hpp"


namespace ex
{
        SkyboxShader::SkyboxShader()
        {
            create_shader_program(m_vert_source_path, m_frag_source_path);
        }

        void SkyboxShader::update_skybox(const Camera* cam)
        {
            use();
            setTexture("skybox", 0);

            // We make the mat4 into a mat3 and then a mat4 again in order to get rid of the last row and column
            // The last row and column affect the translation of the skybox (which we don't want to affect)
            glm::mat4 view = glm::mat4(glm::mat3(cam->get_view_matrix()));
            glm::mat4 projection = cam->get_projection_matrix();

            setMat4("view", view);
            setMat4("projection", projection);
        }
} // namespace ex
