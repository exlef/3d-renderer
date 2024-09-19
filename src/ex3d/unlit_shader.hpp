#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <filesystem>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>
#include <vector>

#include "helper.hpp"
#include "open_gl_error_checking.hpp"
#include "camera.hpp"
#include "base_shader.hpp"

namespace ex
{

    class UnlitShader: public BaseShader
    {
    private:
        std::string m_vert_source_path = "src/ex3d/shaders/unlit.vert";
        std::string m_frag_source_path = "src/ex3d/shaders/unlit.frag";
        
        const Camera* m_cam = nullptr;

    public:
        glm::vec3 color = glm::vec3(1);

        UnlitShader(const Camera* cam);

        void update() override;
    };
} // namespace ex

