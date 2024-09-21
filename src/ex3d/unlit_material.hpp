#pragma once

#include <glm/glm.hpp>

#include "base_entity_material.hpp"
#include "scene.hpp"
#include "shader.hpp"

namespace ex
{

    class UnlitMaterial : public BaseEntityMaterial
    {
    public:
        Shader shader;
        glm::vec3 color = glm::vec3(1);
        UnlitMaterial(glm::vec3 color);
        
        void update(Scene scene, std::string entity_id) override;

    private:
        std::string m_vert_source_path = "src/ex3d/shaders/unlit.vert";
        std::string m_frag_source_path = "src/ex3d/shaders/unlit.frag";
    };
} // namespace ex

