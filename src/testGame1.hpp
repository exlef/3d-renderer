#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <memory>
#include <string>
#include <vector>

#include "ex3d/app.hpp"
#include "ex3d/camera.hpp"
#include "ex3d/texture.hpp"
#include "ex3d/transform.hpp"
#include "fly_cam.hpp"
#include "ex3d/entity_manager.hpp"
#include "ex3d/mesh_comp.hpp"
#include "ex3d/default_shader.hpp"
#include "ex3d/unlit_shader.hpp"

class TestGame1
{
private:
    ex::App app = ex::App(800, 600, "test");
    ex::Camera m_cam = ex::Camera(app.aspect_ratio(), glm::vec3(0, 2, 10));
    FlyCam fly_cam = FlyCam(&m_cam);

#pragma region textures
    ex::Texture m_container_dif_tex = ex::Texture("src/res/textures/container2.png");
    ex::Texture m_container_spec_tex = ex::Texture("src/res/textures/container2_specular.png");
    ex::Texture m_marble_tex = ex::Texture("src/res/textures/marble.jpg");
    ex::Texture m_wood_tex = ex::Texture("src/res/textures/wood.png");
#pragma endregion

public:
    TestGame1()
    {
        app.set_update_callback([this]() { update(); });
        app.set_key_callback([this](int key, int action) { handle_key_callbacks(key, action); });
        app.set_mouse_callback([this](float xpos, float ypos) {handle_mouse_move(xpos, ypos);});
        app.set_window_resize_callback([this](int width, int height) { handle_window_resize(width, height); });

        // m_light_manager.add_sky_light(0);
        // m_light_manager.add_dir_light(glm::vec3(-45, 0, 0), glm::vec3(1,1,1) ,0.2f);
        // m_light_manager.add_point_light(glm::vec3(1, 3, 0), glm::vec3(1, 0, 0), 1);
        // m_light_manager.add_point_light(glm::vec3(-1, 3, 0), glm::vec3(0, 1, 0), 1);
        // m_light_manager.add_point_light(glm::vec3(0, 0, 1.5f), glm::vec3(0, 1, 1), 1);

        app.cam = &m_cam;
        // app.setup_shadow_map(&m_light_manager.dir_light);

        { // sphere
            auto sphere_entt = app.entt_man.add_entity("sphere");
            sphere_entt->mesh = std::make_unique<ex::MeshComponent>("src/res/models/sphere.obj");
            sphere_entt->tr = std::make_unique<ex::Transform>();
            sphere_entt->shader = std::make_unique<ex::DefaultShader>(&m_cam, m_marble_tex.id(), 0);

            sphere_entt->tr->pos = glm::vec3(0, 1, -2);
            sphere_entt->tr->scale = glm::vec3(0.5f);
        }

        { // ground
            auto ground_entt = app.entt_man.add_entity("ground");
            ground_entt->mesh = std::make_unique<ex::MeshComponent>("src/res/models/plane.obj");
            ground_entt->tr = std::make_unique<ex::Transform>();
            ground_entt->shader = std::make_unique<ex::DefaultShader>(&m_cam, m_wood_tex.id(), 0);

            ground_entt->tr->pos = glm::vec3(0, -1, 0);
            ground_entt->tr->scale = glm::vec3(10, 1, 10);
        }

        for (int i = 0; i < 5; i++) // boxes
        {
            auto box_entt = app.entt_man.add_entity("box_" + std::to_string(i));
            box_entt->mesh = std::make_unique<ex::MeshComponent>("src/res/models/cube.obj");
            box_entt->tr = std::make_unique<ex::Transform>();
            box_entt->shader = std::make_unique<ex::DefaultShader>(&m_cam, m_container_dif_tex.id(), m_container_spec_tex.id());

            box_entt->tr->pos = glm::vec3(-5 + i*2, 0, 0);
        }

        for(auto& l : app.get_point_lights())
        {   static int i = 0;
            auto light_entt = app.entt_man.add_entity("p_light_" + std::to_string(i));
            light_entt->mesh = std::make_unique<ex::MeshComponent>("src/res/models/sphere.obj");
            light_entt->tr = std::make_unique<ex::Transform>();
            light_entt->shader = std::make_unique<ex::UnlitShader>(&m_cam);
            // light_entt
        }
        app.run();
    }
private:
    void update()
    {
        fly_cam.move(app);

        // m_sphere.tr.pos.y += sinf(glfwGetTime()) * app.dt();

        // for (size_t i = 0; i < 5; i++)
        // {
            // m_cube.tr.pos.x = (4 - ((int)i * 2));
            // app.draw(m_cube, m_cube_shader);
        // }

        // app.draw(m_sphere, m_sphere_shader);

        // app.draw(m_ground, m_ground_shader);

        // for (size_t i = 0; i < m_light_manager.point_lights.size(); i++)
        // {
        //     m_light.tr.scale = glm::vec3(0.2f);
        //     m_light.tr.pos = m_light_manager.point_lights[i].tr.pos;
        //     // m_light_source_shader.color = m_light_manager.point_lights[i].color;
        //     // app.draw(m_light, m_light_source_shader);
        // }        
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

