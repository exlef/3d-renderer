#pragma once

#include <string>

#include "base_entity_material.hpp"
#include "scene.hpp"
#include "shader.hpp"

namespace ex
{
    class DefaultMaterial : public BaseEntityMaterial
    {
    public:
        Shader shader;
        DefaultMaterial() = delete;
        DefaultMaterial(uint32_t diffuse_texture_id = 0, uint32_t spec_texture_id = 0);

        void set_textures(unsigned int shadow_map_texture_id) const;
        void update(const Scene& scene, std::string entity_id) override;
    
    private:
        std::string m_vert_source_path = "src/ex3d/shaders/default.vert";
        std::string m_frag_source_path = "src/ex3d/shaders/default.frag";
        // https://stackoverflow.com/questions/7322147/what-is-the-range-of-opengl-texture-id
        uint32_t m_diffuse_texture_id = 0;
        uint32_t m_spec_texture_id = 0;
    };
} // namespace ex

