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
        std::string m_vert_source_path = "src/ex3d/shaders/unlit.vert", m_frag_source_path = "src/ex3d/shaders/unlit.frag";

    public:
        UnlitShader()
        {
            create_shader_program(m_vert_source_path, m_frag_source_path);
            if (m_id == 0 || m_id == GL_INVALID_INDEX)
            {
                throw std::runtime_error("Failed to create shader program");
            }

            setup(glm::vec3(1));
        }
        ~UnlitShader() = default;

        unsigned int id()
        {
            return m_id;
        }

        void setup(glm::vec3 color)
        {
            use();
            setVec3("color", color);

            // setFloat("material.shininess", 32.0f);
        }

        void update(ex::Model& model, ex::Camera& cam, glm::vec3 color)
        {
            setup(color);
            set_model_matrix(model.tr.get_model_matrix());

            set_view_matrix(cam.get_view_matrix());

            set_projection_matrix(cam.get_projection_matrix());
        }

        void set_model_matrix(const glm::mat4& mat)
        {
            setMat4("model", mat);
        }

        void set_view_matrix(const glm::mat4& mat)
        {
            setMat4("view", mat);
        }

        void set_projection_matrix(const glm::mat4& mat)
        {
            setMat4("projection", mat);
        }

    };
} // namespace ex

