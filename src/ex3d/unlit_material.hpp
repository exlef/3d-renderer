#pragma once

#include <glm/glm.hpp>

#include "camera.hpp"
#include "shader.hpp"

namespace ex
{

    class UnlitShader
    {
    public:
        Shader shader;
        glm::vec3 color = glm::vec3(1);
        UnlitShader(const Camera* cam);
        
        void update();

    private:
        std::string m_vert_source_path = "src/ex3d/shaders/unlit.vert";
        std::string m_frag_source_path = "src/ex3d/shaders/unlit.frag";
        
        const Camera* m_cam = nullptr;
    };
} // namespace ex

