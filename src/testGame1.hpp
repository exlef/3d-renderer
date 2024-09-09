#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

#include "ex3d/app.hpp"
#include "ex3d/camera.hpp"
#include "ex3d/shader.hpp"
#include "ex3d/texture.hpp"

#include "fly_cam.hpp"

class TestGame1
{
private:
    ex::App app = ex::App(800, 600, "test");
    ex::Camera m_cam = ex::Camera(app.aspect_ratio());
    // std::vector<ex::Model> m_models;
    ex::Model m_cube = ex::Model("src/res/models/cube.obj");
    ex::Model m_sphere = ex::Model("src/res/models/sphere.obj");
    ex::Light m_light_manager;
    ex::Texture m_container_dif_tex = ex::Texture("src/res/textures/container2.png");
    ex::Texture m_container_spec_tex = ex::Texture("src/res/textures/container2_specular.png");
    ex::Texture m_marble_tex = ex::Texture("src/res/textures/marble.jpg");
    ex::Shader m_cube_shader = ex::Shader(m_container_dif_tex.id(), m_container_spec_tex.id(), glm::vec3(0.4f));
    ex::Shader m_sphere_shader = ex::Shader(m_marble_tex.id(), 0, glm::vec3(0.4f));
    FlyCam fly_cam = FlyCam(&m_cam);

public:
    TestGame1()
    {
        app.set_update_callback([this]() { update(); });
        app.set_key_callback([this](int key, int action) { handle_key_callbacks(key, action); });
        app.set_mouse_callback([this](float xpos, float ypos) {handle_mouse_move(xpos, ypos);});
        app.set_window_resize_callback([this](int width, int height) { handle_window_resize(width, height); });

        // m_models.emplace_back(m_cube);
        // m_models.push_back(m_sphere);

        m_light_manager.add_dir_light(glm::vec3(-45, 0, 0), 1);

        m_cube.tr.pos.x = -1;
        m_sphere.tr.pos.x = 1;

        app.run();
    }
private:
    void update()
    {
        fly_cam.move(app);

        m_cube.tr.pos.y -= sinf(glfwGetTime()) * app.dt();
        update_shader(m_cube_shader, m_cube);
        app.draw(m_cube);

        m_sphere.tr.pos.y += sinf(glfwGetTime()) * app.dt();
        update_shader(m_sphere_shader, m_sphere);
        app.draw(m_sphere);
    }

    void update_shader(ex::Shader& s, ex::Model& m)
    {
        s.use();

        s.set_textures();

        s.set_model_matrix(m.tr.get_model_matrix());

        s.set_view_matrix(m_cam.get_view_matrix());
        s.set_view_pos(m_cam.tr.pos);

        s.set_projection_matrix(m_cam.get_projection_matrix());

        s.set_directional_light(m_light_manager.dir_light);
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

    void handle_mouse_move(float xpos, float ypos)
    {
        fly_cam.look_around(xpos, ypos, app.dt());
    }

    void handle_window_resize(int width, int height)
    {
        m_cam.set_aspect_ratio((float)width / (float)height);
    }
};

