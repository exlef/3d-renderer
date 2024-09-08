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
    std::vector<ex::Model> m_models;
    ex::Light m_light_manager;
    ex::Texture m_container_dif_tex = ex::Texture("src/res/textures/container2.png");
    ex::Texture m_container_spec_tex = ex::Texture("src/res/textures/container2_specular.png");
    ex::Shader m_default_shader = ex::Shader(m_container_dif_tex.id(), m_container_spec_tex.id(), glm::vec3(0.4f));
    FlyCam fly_cam = FlyCam(&m_cam);

public:
    TestGame1()
    {
        app.set_update_callback([this]() { update(); });
        app.set_key_callback([this](int key, int action) { handle_key_callbacks(key, action); });
        app.set_mouse_callback([this](float xpos, float ypos) {handle_mouse_move(xpos, ypos);});
        app.set_window_resize_callback([this](int width, int height) { handle_window_resize(width, height); });

        m_models.push_back(ex::Model("src/res/models/cube.obj"));

        m_light_manager.add_dir_light(glm::vec3(-45, 0, 0), 1);

        m_default_shader.use();

        app.run();
    }
private:
    void update()
    {
        // m_cube.tr.rotateY(app.dt() * 50);
        // m_cube.tr.rotateX(app.dt() * 25);

        fly_cam.move(app);

        // m_light_manager.dir_light.tr.rotateX(app.dt() * 10);

        update_shader();
        for(auto& m : m_models)
        {
            app.draw(m);
        }
    }

    void update_shader()
    {
        for (auto& m : m_models)
        {
            m_default_shader.set_model_matrix(m.tr.get_model_matrix());
        }
        // if (m_cube.tr.is_dirty())
        // {
        //     m_default_shader.set_model_matrix(m_cube.tr.get_model_matrix());
        // }
        if(m_cam.tr.is_dirty())
        {
            m_default_shader.set_view_matrix(m_cam.get_view_matrix());
            m_default_shader.set_view_pos(m_cam.tr.get_pos());
        }
        if(m_cam.is_projection_matrix_require_update)
        {
            m_default_shader.set_projection_matrix(m_cam.get_projection_matrix());
        }
        if(m_light_manager.dir_light.tr.is_dirty())
        {
            m_default_shader.set_directional_light(m_light_manager.dir_light);
        }
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

