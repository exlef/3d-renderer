#pragma once

#include <iostream>
#include <string>
#include <vector>

#include "camera.hpp"
#include "texture.hpp"
#include "helper.hpp"
#include "open_gl_error_checking.hpp"
#include "base_shader.hpp"

namespace ex
{
    class DefaultShader : public BaseShader
    {
    private:
        std::string m_vert_source_path = "src/ex3d/shaders/default.vert";
        std::string m_frag_source_path = "src/ex3d/shaders/default.frag";
        
        // https://stackoverflow.com/questions/7322147/what-is-the-range-of-opengl-texture-id
        uint32_t m_diffuse_texture_id = 0;
        uint32_t m_spec_texture_id = 0;

        const Camera* m_cam = nullptr;

    public:
        DefaultShader() = delete;
        DefaultShader(const Camera* cam, uint32_t diffuse_texture_id = 0, uint32_t spec_texture_id = 0);

        void update() override;
        void set_textures() const;
    };
} // namespace ex

