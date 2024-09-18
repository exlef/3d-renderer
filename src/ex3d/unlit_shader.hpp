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

        UnlitShader(const Camera* cam)
        {
            create_shader_program(m_vert_source_path, m_frag_source_path);

            m_cam = cam;

            use();
            setVec3("color", color);
        }

        virtual void update(const glm::mat4& model_mat, const glm::mat4& view_mat, const glm::mat4& proj_mat,
                            glm::vec3 cam_pos,
                            glm::vec3 entity_pos,
                            SkyLight sky_light,
                            DirectionalLight dir_light,
                            std::vector<PointLight*> point_lights) 
        { 
            UNUSED(model_mat);
            UNUSED(view_mat);
            UNUSED(proj_mat);
            UNUSED(cam_pos );
            UNUSED(entity_pos);
            UNUSED(sky_light);
            UNUSED(dir_light);
            UNUSED(point_lights);

            use();

            setMat4("model", model_mat);
            setMat4("view", view_mat);
            setMat4("projection", proj_mat);
            setVec3("color", color);
        }

        // void update(const App& app, const Entity& entity) override
        /*void update(const App& app, const Entity& entity) override
        {
            UNUSED(app);
            use();

            setMat4("model", entity.tr->get_model_matrix());
            setMat4("view", m_cam->get_view_matrix());
            setMat4("projection", m_cam->get_projection_matrix());
            setVec3("color", color);
        }*/

    };
} // namespace ex

