#pragma once

#include <string>

#include "skybox_shader.hpp"
#include "camera.hpp"

namespace ex
{
    class Skybox
    {
    public:
        unsigned int skyboxVAO;
        unsigned int cubemapTexture;
        
        Skybox();
        void update_shader(const Camera* cam);

    private:
        SkyboxShader m_skybox_shader;
        unsigned int loadCubemap(std::vector<std::string> faces);
    };
    
} // namespace ex
