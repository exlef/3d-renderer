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
        app.set_update_callback([this]() { update(); });
        app.set_key_callback([this](int key, int action) { handle_key_callbacks(key, action); });
        // app.set_window_resize_callback([this](int width, int height) { handle_framebuffer_size_callback(width, height); });
    }
    ~Game() = default;

    void run()
    {
        app.run();
    }
private:
    void update()
    {
        if (ex::is_key_down(app.window(), KEY_W))
        {
            std::cout << "W is being pressed" << std::endl;
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

    void handle_framebuffer_size_callback(int width, int height)
    {
        std::cout<< width << std::endl;
    }
};

