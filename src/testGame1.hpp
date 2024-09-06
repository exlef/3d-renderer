#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "ex3d/app.hpp"
#include "ex3d/camera.hpp"
#include "ex3d/shader.hpp"
#include "ex3d/texture.hpp"

class TestGame1
{
private:
    ex::App app = ex::App(800, 600, "test");
    ex::Camera m_cam = ex::Camera();
    ex::Model m_cube = ex::Model("src/res/models/cube.obj");
    ex::Shader m_default_shader = ex::Shader("src/shaders/default.vert", "src/shaders/default.frag");
    ex::Texture m_cube_tex = ex::Texture("src/res/textures/container2.png");

    // float cube_angle_y = 0;
public:
    TestGame1()
    {
        app.set_update_callback([this]() { update(); });
        app.set_key_callback([this](int key, int action) { handle_key_callbacks(key, action); });
        app.set_window_resize_callback([this](int width, int height) { handle_window_resize(width, height); });

        m_default_shader.use();
        m_default_shader.set_model_matrix(m_cube.get_model_matrix());
        m_default_shader.set_view_matrix(m_cam.get_view_matrix());
        m_default_shader.set_projection_matrix(m_cam.get_projection_matrix(app.aspect_ratio()));
        m_default_shader.set_textures(m_cube_tex.id());

        m_cube.set_model_matrix_changed_callback([this](glm::mat4 mat) { m_default_shader.set_model_matrix(mat); });

        app.run();
    }
    ~TestGame1() = default;

private:
    void update()
    {
        m_cube.tr.rotateY(app.dt() * 50);
        m_cube.tr.rotateX(app.dt() * 25);
        // m_cube.scale(app.dt() * 2);
        // m_cube.scaleX(app.dt() * 2);
        // m_cube.scaleY(app.dt() * 2);
        // m_cube.scaleZ(app.dt() * 2);
        m_cube.tr.set_pos(glm::vec3(1,1,1));
        m_cube.tr.set_scale(0.5f);

        app.draw(m_cube);
    }

    void handle_key_callbacks(int key, int action)
    {
        if (action == KEY_PRESS)
        {
            if (key == KEY_ESCAPE)
            {
                app.quit();
            }
        }
    }

    void handle_window_resize(int width, int height)
    {
        m_default_shader.set_projection_matrix(m_cam.get_projection_matrix((float)width / (float)height));
    }
};

