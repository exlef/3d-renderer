#pragma once

#include <glm/glm.hpp>

#include "ex3d/app.hpp"
#include "ex3d/camera.hpp"
#include "ex3d/texture.hpp"



class TestGame1 : public ex::App
{
#pragma region textures
    ex::Texture m_container_dif_tex = ex::Texture("src/res/textures/container2.png");
    ex::Texture m_container_spec_tex = ex::Texture("src/res/textures/container2_specular.png");
    ex::Texture m_marble_tex = ex::Texture("src/res/textures/marble.jpg");
    ex::Texture m_wood_tex = ex::Texture("src/res/textures/wood.png");
#pragma endregion

public:
    TestGame1();

private:
    float last_x = 0;
    float last_y = 0;
    bool is_first = true;
    float cam_speed = 10;
    float sensitivity = 5;
private:
    void on_update() override;

    void on_key_callbacks(int key, int action) override;

    void on_mouse_move(float xpos, float ypos) override;

    void on_framebuffer_resize(int width, int height) override;
};

