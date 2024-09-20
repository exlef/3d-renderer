#include "testGame1.hpp"


#include "ex3d/default_material.hpp"
#include "ex3d/unlit_material.hpp"
#include "ex3d/entity_manager.hpp"
#include "ex3d/mesh_comp.hpp"
#include "ex3d/transform.hpp"


TestGame1::TestGame1() : App(800, 600, "game")
{
    { // sphere
        auto sphere_entt = ex::entt_man.add_entity("sphere");
        sphere_entt->mesh = std::make_unique<ex::MeshComponent>("src/res/models/sphere.obj");
        sphere_entt->tr = std::make_unique<ex::Transform>();
        // sphere_entt->shader = std::make_unique<ex::DefaultShader>(&cam, m_marble_tex.id(), 0);

        sphere_entt->tr->pos = glm::vec3(0, 1, -2);
        sphere_entt->tr->scale = glm::vec3(0.5f);
    }

    { // ground
        auto ground_entt = ex::entt_man.add_entity("ground");
        ground_entt->mesh = std::make_unique<ex::MeshComponent>("src/res/models/plane.obj");
        ground_entt->tr = std::make_unique<ex::Transform>();
        // ground_entt->shader = std::make_unique<ex::DefaultShader>(&cam, m_wood_tex.id(), 0);

        ground_entt->tr->pos = glm::vec3(0, -1, 0);
        ground_entt->tr->scale = glm::vec3(10, 1, 10);
    }

    for (int i = 0; i < 5; i++) // boxes
    {
        auto box_entt = ex::entt_man.add_entity("box_" + std::to_string(i));
        box_entt->mesh = std::make_unique<ex::MeshComponent>("src/res/models/cube.obj");
        box_entt->tr = std::make_unique<ex::Transform>();
        // box_entt->shader = std::make_unique<ex::DefaultShader>(&cam, m_container_dif_tex.id(), m_container_spec_tex.id());

        box_entt->tr->pos = glm::vec3(-5 + i*2, 0, 0);
    }

    // static int i = 0;
    // auto light_entt = app.entt_man.add_entity("p_light_" + std::to_string(i));
    // light_entt->mesh = std::make_unique<ex::MeshComponent>("src/res/models/sphere.obj");
    // light_entt->tr = std::make_unique<ex::Transform>();
    // light_entt->shader = std::make_unique<ex::UnlitShader>(&m_cam);
    // light_entt

    run();
}

void TestGame1::on_update()
{

    if (is_key_down(KEY_E))
        cam.tr.pos. y += (dt * cam_speed);
    if (is_key_down(KEY_Q))
        cam.tr.pos.y += (dt * -cam_speed);
    if (is_key_down(KEY_W))
        cam.tr.local_translateZ(dt * cam_speed);
    if (is_key_down(KEY_S))
        cam.tr.local_translateZ(dt * -cam_speed);
    if (is_key_down(KEY_A))
        cam.tr.local_translateX(dt * -cam_speed);
    if (is_key_down(KEY_D))
        cam.tr.local_translateX(dt * cam_speed);
}

void TestGame1::on_key_callbacks(int key, int action)
{
    if (action == KEY_PRESS)
    {
        if (key == KEY_ESCAPE)
        {
            quit();
        }
    }
}

void TestGame1::on_mouse_move(float xpos, float ypos)
{
    if (is_first)
    {
        last_x = xpos;
        last_y = ypos;
        is_first = false;
    }
    float dx = (last_x - xpos) * sensitivity;
    float dy = (last_y - ypos) * sensitivity;
    last_x = xpos;
    last_y = ypos;
    cam.tr.local_rotateY(dx * dt);
    cam.tr.local_rotateX(dy * dt);
}

void TestGame1::on_framebuffer_resize(int width, int height)
{
    cam.aspect_ratio = (float)width / (float)height;
}

