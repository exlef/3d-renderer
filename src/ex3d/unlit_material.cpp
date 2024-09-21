#include "unlit_material.hpp"


#include "entity_manager.hpp"
#include "glm/fwd.hpp"

namespace ex 
{
    UnlitMaterial::UnlitMaterial()
    {
        shader.create_shader_program(m_vert_source_path, m_frag_source_path);

        shader.use();
        shader.setVec3("color", glm::vec3(1,0,1));
    }

    void UnlitMaterial::update(Scene scene, std::string entity_id)
    { 
        shader.use();

        shader.setMat4("model", ex::entt_man.get_entity(entity_id)->tr->get_model_matrix());
        shader.setMat4("view", scene.camera->get_view_matrix());
        shader.setMat4("projection", scene.camera->get_projection_matrix());
        shader.setVec3("color", entt_man.get_entity(entity_id)->point_light->color);
    }
}