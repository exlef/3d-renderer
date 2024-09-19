#include "default_shader.hpp"


#include <GL/glew.h>
#include <GLFW/glfw3.h>

namespace ex 
{
        DefaultShader::DefaultShader(const Camera* cam, uint32_t diffuse_texture_id, uint32_t spec_texture_id)
        {
            create_shader_program(m_vert_source_path, m_frag_source_path);
            
            m_cam = cam;

            m_diffuse_texture_id = diffuse_texture_id;
            m_spec_texture_id = spec_texture_id;

            use();
            set_textures();
            setFloat("material.shininess", 32.0f);
        }

        void DefaultShader::update() 
        { 
            // use();

            // set_textures();

            // setMat4("model", model_mat);

            // setMat4("view", view_mat);

            // setVec3("viewPos", cam_pos);

            // setMat4("projection", proj_mat);

            // setFloat("skyLight", sky_light.strength);

            // setVec3("dirLight.direction", dir_light.tr.get_forward());
            // setVec3("dirLight.color", dir_light.color);
            // setFloat("dirLight.strength", dir_light.strength);

            // size_t point_light_count = point_lights.size();
            // setInt("pointLightCount", point_light_count);

            // for (size_t i = 0; i < point_light_count; i++)
            // {
            //     const PointLight* light =  point_lights[i];

            //     std::string index_str = std::to_string(i);
            //     setVec3("pointLights[" + index_str + "].position", entity_pos);
            //     setVec3("pointLights[" + index_str + "].color", light->color);
            //     setFloat("pointLights[" + index_str + "].strength", light->strength);
            //     setFloat("pointLights[" + index_str + "].constant", light->constant);
            //     setFloat("pointLights[" + index_str + "].linear", light->linear);
            //     setFloat("pointLights[" + index_str + "].quadratic", light->quadratic);
            // }
        }

        void DefaultShader::set_textures() const
        {
            // set a default color for diffuse in case there is no textures provided
            if (m_diffuse_texture_id == 0)
            {
                ex::Texture m_default_texture = ex::Texture("src/ex3d/res/default2.png");
                glc(glActiveTexture(GL_TEXTURE0));
                glc(glBindTexture(GL_TEXTURE_2D, m_default_texture.id()));
                glc(glUniform1i(glGetUniformLocation(m_id, "material.diffuse"), 0));
            }
            else
            {
                glc(glActiveTexture(GL_TEXTURE0));
                glc(glBindTexture(GL_TEXTURE_2D, m_diffuse_texture_id));
                glc(glUniform1i(glGetUniformLocation(m_id, "material.diffuse"), 0));
            }

            if (m_spec_texture_id != 0)
            {
                glc(glActiveTexture(GL_TEXTURE1));
                glc(glBindTexture(GL_TEXTURE_2D, m_spec_texture_id));
                glc(glUniform1i(glGetUniformLocation(m_id, "material.specular"), 1));
            }
        }
}