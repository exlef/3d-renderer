#include "ex3d/app.hpp"
#include "ex3d/input.hpp"

void key_callbacks(GLFWwindow* window, int key, int scancode, int action, int mods);

static ex::app app;

int main()
{
    app.create_window(800, 600, "test");

    ex::set_key_callback(app.window(), key_callbacks);

    while (app.running())
    {
        app.start_drawing();

        if (ex::is_key_down(app.window(), KEY_W))
        {
            std::cout << "W is being pressed" << std::endl;
        }

        app.end_drawing();
    }
}

void key_callbacks(__attribute__((unused)) GLFWwindow* window, int key, __attribute__((unused)) int scancode, int action, __attribute__((unused)) int mods)
{
    if (action == GLFW_PRESS)
    {
        app.quit();
    }
}
