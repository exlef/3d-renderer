#include "unlit_material.hpp"


#include "entity_manager.hpp"

namespace ex 
{
    UnlitMaterial::UnlitMaterial(glm::vec3 color)
    {
        shader.create_shader_program(m_vert_source_path, m_frag_source_path);

        shader.use();
        this->color = color;
        shader.setVec3("color", color);
    }

    void UnlitMaterial::update(Scene scene, std::string entity_id)
    { 
        shader.use();

        shader.setMat4("model", ex::entt_man.get_entity(entity_id)->tr->get_model_matrix());
        shader.setMat4("view", scene.camera->get_view_matrix());
        shader.setMat4("projection", scene.camera->get_projection_matrix());
        shader.setVec3("color", color);
    }
}