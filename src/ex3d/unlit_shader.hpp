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

#include "open_gl_error_checking.hpp"
#include "camera.hpp"

namespace ex
{

    class UnlitShader: public BaseShader
    {
    private:
        std::string m_vert_source_path = "src/ex3d/shaders/unlit.vert";
        std::string m_frag_source_path = "src/ex3d/shaders/unlit.frag";
        
        const Camera* m_cam = nullptr;
        const Transform* m_tr = nullptr;

    public:
        glm::vec3 color = glm::vec3(1);

        UnlitShader(const Camera* cam, const Transform* tr)
        {
            create_shader_program(m_vert_source_path, m_frag_source_path);

            m_cam = cam;
            m_tr = tr;

            use();
            setVec3("color", color);
        }

        void update() override
        {
            use();

            setMat4("model", m_tr->get_model_matrix());
            setMat4("view", m_cam->get_view_matrix());
            setMat4("projection", m_cam->get_projection_matrix());
            setVec3("color", color);
        }

    };
} // namespace ex

