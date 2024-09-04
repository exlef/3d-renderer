#pragma once

#include "ex3d/app.hpp"
#include "ex3d/input.hpp"

class Game
{
private:
    ex::App app = ex::App(800, 600, "test");
public:
    Game()
    {
        app.set_update_callback([this](float dt) { update(dt); });
        // ex::set_key_callback(app.window(), key_callbacks);
        // app.set_resize_callback(framebuffer_size_callback);
        // Use lambdas for key and framebuffer size callbacks

        // Set the user pointer to the current instance of Game
        // glfwSetWindowUserPointer(app.window(), this);

        app.set_key_callback([this](int key, int action) { handle_key_callbacks(key, action); });
        app.set_resize_callback([this](int width, int height) { handle_framebuffer_size_callback(width, height); });
    }
    ~Game();

private:
    void update(float dt)
    {
        if (ex::is_key_down(app.window(), KEY_W))
        {
            std::cout << "W is being pressed" << std::endl;
        }
    }
    // static void key_callbacks(GLFWwindow* window, int key,  int scancode, int action, __attribute__((unused)) int mods)
    // {
    //     // Retrieve the Game instance from the user pointer
    //     Game* game = static_cast<Game*>(glfwGetWindowUserPointer(window));
    //     if (game)
    //     {
    //         game->handle_key_callbacks(window, key, scancode, action, mods);
    //     }
    // }

    // static void framebuffer_size_callback(__attribute__((unused)) GLFWwindow* window, int width, int height)
    // {
    //     // Retrieve the Game instance from the user pointer
    //     Game* game = static_cast<Game*>(glfwGetWindowUserPointer(window));
    //     if (game)
    //     {
    //         game->handle_framebuffer_size_callback(window, width, height);
    //     }
    // }

    void handle_key_callbacks(int key, int action)
    {
        if (action == KEY_PRESS)
        {
            if (key == KEY_ESCAPE)
            {
                // app.quit();
            }
        }
    }

    void handle_framebuffer_size_callback(int width, int height)
    {
        glViewport(0, 0, width, height);
    }
};

