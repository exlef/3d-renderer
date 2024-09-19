#pragma once

#include "base_shader.hpp"
#include "camera.hpp"

namespace ex
{
    class SkyboxShader : BaseShader
    {
    public:
        SkyboxShader();
        void update_skybox(const Camera* cam);
        
    private:
        std::string m_vert_source_path = "src/ex3d/shaders/skybox.vert";
        std::string m_frag_source_path = "src/ex3d/shaders/skybox.frag";
    };
} // namespace ex
