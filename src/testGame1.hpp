#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
    ex::Model m_cube = ex::Model("src/res/models/cube.obj");
    ex::Light m_light_manager;
    ex::Texture m_container_dif_tex = ex::Texture("src/res/textures/container2.png");
    ex::Texture m_container_spec_tex = ex::Texture("src/res/textures/container2_specular.png");
    ex::Shader m_default_shader = ex::Shader(m_container_dif_tex.id(), m_container_spec_tex.id(), glm::vec3(0.4f));
    // float last_x = 0;
    // float last_y = 0;
    // bool is_first = true;
    // float sensitivity = 5;
    FlyCam fly_cam = FlyCam(&m_cam);

public:
    TestGame1()
    {
        app.set_update_callback([this]() { update(); });
        app.set_key_callback([this](int key, int action) { handle_key_callbacks(key, action); });
        app.set_mouse_callback([this](float xpos, float ypos) {handle_mouse_move(xpos, ypos);});
        app.set_window_resize_callback([this](int width, int height) { handle_window_resize(width, height); });

        // TODO: when direction light's direction changed we need to update the material.
        m_light_manager.add_dir_light(glm::vec3(-2.0f), 1);

        m_default_shader.use();
        m_default_shader.set_directional_light(m_light_manager.dir_light);

        app.run();
    }
private:
    void update()
    {
        // m_cube.tr.rotateY(app.dt() * 50);
        // m_cube.tr.rotateX(app.dt() * 25);

        // m_cam.move(app);
        fly_cam.move(app);

        update_shader();
        app.draw(m_cube);
    }

    void update_shader()
    {
        if (m_cube.tr.is_dirty())
        {
            m_default_shader.set_model_matrix(m_cube.tr.get_model_matrix());
        }
        if(m_cam.tr.is_dirty())
        {
            m_default_shader.set_view_matrix(m_cam.get_view_matrix());
            m_default_shader.set_view_pos(m_cam.tr.get_pos());
        }
        if(m_cam.is_projection_matrix_require_update)
        {
            m_default_shader.set_projection_matrix(m_cam.get_projection_matrix());
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
        // if(is_first) 
        // {
        //     last_x = xpos;
        //     last_y = ypos;
        //     is_first = false;
        // }
        // float dx = (last_x - xpos) * sensitivity;
        // float dy = (last_y - ypos) * sensitivity;
        // last_x = xpos;
        // last_y = ypos;
        // m_cam.tr.local_rotateY(dx * app.dt());
        // m_cam.tr.local_rotateX(dy * app.dt());
        // m_cam.look_around(xpos, ypos);
        fly_cam.look_around(xpos, ypos, app.dt());
    }

    void handle_window_resize(int width, int height)
    {
        m_cam.set_aspect_ratio((float)width / (float)height);
    }
};

