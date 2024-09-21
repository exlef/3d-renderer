#pragma once

#include <string>

#include "shader.hpp"
#include "camera.hpp"

namespace ex
{
    class Skybox
    {
    public:
        Shader shader;
        unsigned int skyboxVAO;
        unsigned int cubemapTexture;
        
        Skybox();
        void draw(const Camera* cam);

    private:
        std::string m_vert_source_path = "src/ex3d/shaders/skybox.vert";
        std::string m_frag_source_path = "src/ex3d/shaders/skybox.frag";

        unsigned int loadCubemap(std::vector<std::string> faces);
    };
    
} // namespace ex
