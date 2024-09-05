#pragma once

#include "ex3d/app.hpp"
#include "ex3d/input.hpp"
#include "ex3d/model.hpp"
#include "ex3d/shader.hpp"
#include "ex3d/camera.hpp"
#include "ex3d/texture.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class TestGame1
{
private:
    ex::App app = ex::App(800, 600, "test");
    ex::Camera m_cam = ex::Camera();
    ex::Model m_cube = ex::Model("src/res/models/cube.obj");
    ex::Shader m_default_shader = ex::Shader("src/shaders/default.vert", "src/shaders/default.frag");
    ex::Texture m_cube_tex = ex::Texture("src/res/textures/container2.png");

public:
    TestGame1()
    {
        app.set_update_callback([this]() { update(); });
        app.set_key_callback([this](int key, int action) { handle_key_callbacks(key, action); });
        app.set_window_resize_callback([this](int width, int height) { handle_window_resize(width, height); });

        m_default_shader.use();
        m_default_shader.set_projection_matrix(m_cam.get_projection_matrix(app.aspect_ratio()));
        m_default_shader.set_view_matrix(m_cam.get_view_matrix());
        m_default_shader.set_textures(m_cube_tex.id());
    }
    ~TestGame1() = default;

    void run()
    {
        app.run();
    }
private:
    void update()
    {
        glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        model = glm::translate(model, glm::vec3(1.0,1.0,1.0));
        model = glm::rotate(model, glm::radians(15.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        m_default_shader.setMat4("model", model);
        
        m_cube.Draw();
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

