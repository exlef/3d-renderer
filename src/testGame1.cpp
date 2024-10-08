#include "testGame1.hpp"


#include "ex3d/default_material.hpp"
#include "ex3d/lights.hpp"
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
        sphere_entt->material = std::make_unique<ex::DefaultMaterial>(m_marble_tex.id(), 0);
        sphere_entt->tr->pos = glm::vec3(0, 1, -2);
        sphere_entt->tr->scale = glm::vec3(0.5f);
    }

    { // ground
        auto ground_entt = ex::entt_man.add_entity("ground");
        ground_entt->mesh = std::make_unique<ex::MeshComponent>("src/res/models/plane.obj");
        ground_entt->tr = std::make_unique<ex::Transform>();
        ground_entt->material = std::make_unique<ex::DefaultMaterial>(m_wood_tex.id(), 0);

        ground_entt->tr->pos = glm::vec3(0, -1, 0);
        ground_entt->tr->scale = glm::vec3(10, 1, 10);
    }

    for (int i = 0; i < 5; i++) // boxes
    {
        auto box_entt = ex::entt_man.add_entity("box_" + std::to_string(i));
        box_entt->mesh = std::make_unique<ex::MeshComponent>("src/res/models/cube.obj");
        box_entt->tr = std::make_unique<ex::Transform>();
        box_entt->material = std::make_unique<ex::DefaultMaterial>(m_container_dif_tex.id(), m_container_spec_tex.id());

        box_entt->tr->pos = glm::vec3(-4 + i*2, 0, 0);
    }

    { // point-light
        auto light_entt = ex::entt_man.add_entity("p_light_red");
        light_entt->mesh = std::make_unique<ex::MeshComponent>("src/res/models/sphere.obj");
        light_entt->tr = std::make_unique<ex::Transform>();
        light_entt->material = std::make_unique<ex::UnlitMaterial>();
        light_entt->point_light = std::make_unique<ex::PointLight>(glm::vec3(1,0,0));

        light_entt->tr->scale = glm::vec3(0.2);
        light_entt->tr->pos = glm::vec3(0,0,2);
    }

    scene.dir_light.tr.local_rotateX(-90);
    // scene.dir_light.tr.local_rotateY(45);

    scene.camera.tr.pos = glm::vec3(0, 3, 10);

    run();
}

void TestGame1::on_update()
{
    ex::entt_man.get_entity("sphere")->tr->pos.y += sinf(get_time()) * dt;

    if (is_key_down(KEY_E))
        scene.camera.tr.pos.y += (dt * cam_speed);
    if (is_key_down(KEY_Q))
        scene.camera.tr.pos.y += (dt * -cam_speed);
    if (is_key_down(KEY_W))
        scene.camera.tr.local_translateZ(dt * cam_speed);
    if (is_key_down(KEY_S))
        scene.camera.tr.local_translateZ(dt * -cam_speed);
    if (is_key_down(KEY_A))
        scene.camera.tr.local_translateX(dt * -cam_speed);
    if (is_key_down(KEY_D))
        scene.camera.tr.local_translateX(dt * cam_speed);
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
    scene.camera.tr.local_rotateY(dx * dt);
    scene.camera.tr.local_rotateX(dy * dt);
}

void TestGame1::on_framebuffer_resize(int width, int height)
{
    scene.camera.aspect_ratio = (float)width / (float)height;
}

