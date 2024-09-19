#include "testGame1.hpp"


#include "ex3d/default_shader.hpp"
#include "ex3d/unlit_shader.hpp"
#include "ex3d/entity_manager.hpp"
#include "ex3d/mesh_comp.hpp"
#include "ex3d/transform.hpp"


TestGame1::TestGame1()
{
    app.set_update_callback([this]() { update(); });
    app.set_key_callback([this](int key, int action) { handle_key_callbacks(key, action); });
    app.set_mouse_callback([this](float xpos, float ypos) {handle_mouse_move(xpos, ypos);});
    app.set_window_resize_callback([this](int width, int height) { handle_window_resize(width, height); });

    app.cam = &m_cam;

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

    // static int i = 0;
    // auto light_entt = app.entt_man.add_entity("p_light_" + std::to_string(i));
    // light_entt->mesh = std::make_unique<ex::MeshComponent>("src/res/models/sphere.obj");
    // light_entt->tr = std::make_unique<ex::Transform>();
    // light_entt->shader = std::make_unique<ex::UnlitShader>(&m_cam);
    // light_entt

    app.run();
}

void TestGame1::update()
{
    fly_cam.move(app);
}

void TestGame1::handle_key_callbacks(int key, int action)
{
    if (action == KEY_PRESS)
    {
        if (key == KEY_ESCAPE)
        {
            app.quit();
        }
    }
}

void TestGame1::handle_mouse_move(float xpos, float ypos)
{
    fly_cam.look_around(xpos, ypos, app.dt());
}

void TestGame1::handle_window_resize(int width, int height)
{
    m_cam.aspect_ratio = (float)width / (float)height;
}

