#pragma once

#include <glm/glm.hpp>

#include "ex3d/app.hpp"
#include "ex3d/camera.hpp"
#include "ex3d/texture.hpp"
#include "fly_cam.hpp"



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
    TestGame1();
private:
    void update();

    void handle_key_callbacks(int key, int action);

    void handle_mouse_move(float xpos, float ypos);

    void handle_window_resize(int width, int height);
};

