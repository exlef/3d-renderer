#pragma once

#include "ex3d/app.hpp"
#include "ex3d/input.hpp"
#include "ex3d/model.hpp"
#include "ex3d/shader.hpp"
#include "ex3d/camera.hpp"
#include "ex3d/texture.hpp"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class TestGame1
{
private:
    ex::App app = ex::App(800, 600, "test");
    ex::Camera m_cam = ex::Camera();
    ex::Model m_cube = ex::Model("src/res/cube.obj");
    ex::Shader m_default_shader = ex::Shader("src/shaders/default.vert", "src/shaders/default.frag");
    ex::Texture m_cube_tex = ex::Texture("src/res/textures/container2.png");

public:
    TestGame1()
    {
        app.set_update_callback([this]() { update(); });
        app.set_key_callback([this](int key, int action) { handle_key_callbacks(key, action); });
    }
    ~TestGame1() = default;

    void run()
    {
        app.run();
    }
private:
    void update()
    {
        m_default_shader.use();

        // bind textures on corresponding texture units
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, m_cube_tex.id());

        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)800 / (float)600, 0.1f, 100.0f);
        m_default_shader.setMat4("projection", projection);

        // camera/view transformation
        glm::mat4 view = m_cam.get_view_matrix();
        m_default_shader.setMat4("view", view);

        glm::mat4 model = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        model = glm::translate(model, glm::vec3(1.0,1.0,1.0));
        model = glm::rotate(model, glm::radians(15.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        m_default_shader.setMat4("model", model);
        
        m_cube.Draw();
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

};

