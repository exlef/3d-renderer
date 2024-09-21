#include "default_material.hpp"


#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "entity_manager.hpp"
#include "open_gl_error_checking.hpp"
#include "texture.hpp"



namespace ex 
{
        DefaultMaterial::DefaultMaterial(uint32_t diffuse_texture_id, uint32_t spec_texture_id)
        {
            shader.create_shader_program(m_vert_source_path, m_frag_source_path);
            
            m_diffuse_texture_id = diffuse_texture_id;
            m_spec_texture_id = spec_texture_id;

            shader.use();
            set_textures();
            shader.setFloat("material.shininess", 32.0f);
        }

        void DefaultMaterial::update(Scene scene, std::string entity_id) 
        { 
            shader.use();

            set_textures();

            shader.setMat4("model", ex::entt_man.get_entity(entity_id)->tr->get_model_matrix());

            shader.setMat4("view", scene.camera.get_view_matrix());

            shader.setVec3("viewPos", scene.camera.tr.pos);

            shader.setMat4("projection", scene.camera.get_projection_matrix());

            shader.setFloat("skyLight", scene.sky_light.strength);

            shader.setVec3("dirLight.direction", scene.dir_light.tr.get_forward());
            shader.setVec3("dirLight.color", scene.dir_light.color);
            shader.setFloat("dirLight.strength", scene.dir_light.strength);

            size_t point_light_count = scene.point_lights.size();
            shader.setInt("pointLightCount", point_light_count);

            for (size_t i = 0; i < point_light_count; i++)
            {
                
                const glm::vec3 light_pos = std::get<0>(scene.point_lights[i]);
                const PointLight* light =  std::get<1>(scene.point_lights[i]);

                std::string index_str = std::to_string(i);
                shader.setVec3("pointLights[" + index_str + "].position", light_pos);
                shader.setVec3("pointLights[" + index_str + "].color", light->color);
                shader.setFloat("pointLights[" + index_str + "].strength", light->strength);
                shader.setFloat("pointLights[" + index_str + "].constant", light->constant);
                shader.setFloat("pointLights[" + index_str + "].linear", light->linear);
                shader.setFloat("pointLights[" + index_str + "].quadratic", light->quadratic);
            }
        }

        void DefaultMaterial::set_textures() const
        {
            // set a default color for diffuse in case there is no textures provided
            if (m_diffuse_texture_id == 0)
            {
                ex::Texture m_default_texture = ex::Texture("src/ex3d/res/default2.png");
                glc(glActiveTexture(GL_TEXTURE0));
                glc(glBindTexture(GL_TEXTURE_2D, m_default_texture.id()));
                glc(glUniform1i(glGetUniformLocation(shader.id(), "material.diffuse"), 0));
            }
            else
            {
                glc(glActiveTexture(GL_TEXTURE0));
                glc(glBindTexture(GL_TEXTURE_2D, m_diffuse_texture_id));
                glc(glUniform1i(glGetUniformLocation(shader.id(), "material.diffuse"), 0));
            }

            if (m_spec_texture_id != 0)
            {
                glc(glActiveTexture(GL_TEXTURE1));
                glc(glBindTexture(GL_TEXTURE_2D, m_spec_texture_id));
                glc(glUniform1i(glGetUniformLocation(shader.id(), "material.specular"), 1));
            }
        }
}