#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

#include "ex3d/app.hpp"
#include "ex3d/camera.hpp"
#include "ex3d/default_shader.hpp"
#include "ex3d/texture.hpp"

#include "fly_cam.hpp"
#include "ex3d/unlit_shader.hpp"

class TestGame1
{
private:
    ex::App app = ex::App(800, 600, "test");
    ex::Camera m_cam = ex::Camera(app.aspect_ratio(), glm::vec3(0, 2, 10));
    ex::Light m_light_manager;
    FlyCam fly_cam = FlyCam(&m_cam);

#pragma region models
    ex::Model m_cube = ex::Model("src/res/models/cube.obj");
    ex::Model m_sphere = ex::Model("src/res/models/sphere.obj");
    ex::Model m_ground = ex::Model("src/res/models/cube.obj");
    ex::Model m_light = ex::Model("src/res/models/sphere.obj");
#pragma endregion
#pragma region textures
    ex::Texture m_container_dif_tex = ex::Texture("src/res/textures/container2.png");
    ex::Texture m_container_spec_tex = ex::Texture("src/res/textures/container2_specular.png");
    ex::Texture m_marble_tex = ex::Texture("src/res/textures/marble.jpg");
    ex::Texture m_wood_tex = ex::Texture("src/res/textures/wood.png");
#pragma endregion
#pragma region shaders
    ex::DefaultShader m_cube_shader = ex::DefaultShader(m_container_dif_tex.id(), m_container_spec_tex.id());
    ex::DefaultShader m_sphere_shader = ex::DefaultShader(m_marble_tex.id(), 0);
    ex::DefaultShader m_ground_shader = ex::DefaultShader(m_wood_tex.id(), 0);
    ex::UnlitShader m_light_source_shader = ex::UnlitShader();
#pragma endregion

public:
    TestGame1()
    {
        app.set_update_callback([this]() { update(); });
        app.set_key_callback([this](int key, int action) { handle_key_callbacks(key, action); });
        app.set_mouse_callback([this](float xpos, float ypos) {handle_mouse_move(xpos, ypos);});
        app.set_window_resize_callback([this](int width, int height) { handle_window_resize(width, height); });

        m_light_manager.add_sky_light(0);
        m_light_manager.add_dir_light(glm::vec3(-45, 0, 0), glm::vec3(1,1,1) ,0.2f);
        // m_light_manager.add_point_light(glm::vec3(1, 3, 0), glm::vec3(1, 0, 0), 1);
        // m_light_manager.add_point_light(glm::vec3(-1, 3, 0), glm::vec3(0, 1, 0), 1);
        m_light_manager.add_point_light(glm::vec3(0, 0, 1.5f), glm::vec3(0, 1, 1), 1);

        m_sphere.tr.pos.z = -2;
        m_ground.tr.pos = glm::vec3(0, -1, 0);
        m_ground.tr.scale = glm::vec3(10, 0.1f, 10);

        // move these into app class?
        app.cam = &m_cam;
        app.setup_shadow_map(&m_light_manager.dir_light);

        app.run();
    }
private:
    void update()
    {
        fly_cam.move(app);

        m_sphere.tr.pos.y += sinf(glfwGetTime()) * app.dt();

        for (size_t i = 0; i < 5; i++)
        {
            m_cube.tr.pos.x = (4 - ((int)i * 2));
            draw_model_with_default_shader(m_cube, m_cube_shader);
        }

        draw_model_with_default_shader(m_sphere, m_sphere_shader);

        draw_model_with_default_shader(m_ground, m_ground_shader);

        for (size_t i = 0; i < m_light_manager.point_lights.size(); i++)
        {
            m_light.tr.scale = glm::vec3(0.2f);
            m_light.tr.pos = m_light_manager.point_lights[i].tr.pos;
            draw_model_with_unlit_shader(m_light, m_light_source_shader, i);
        }        
    }

    void draw_model_with_default_shader(ex::Model& model, ex::DefaultShader& shader)
    {
        shader.update(model, m_cam, m_light_manager);
        app.draw(model, shader.id());
    }

    void draw_model_with_unlit_shader(ex::Model& model, ex::UnlitShader& shader, int point_light_index)
    {
        shader.update(model, m_cam, m_light_manager.point_lights[point_light_index].color);
        app.draw(model, shader.id());
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

