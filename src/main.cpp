#include "ex3d/app.hpp"
#include "ex3d/input.hpp"

void key_callbacks(GLFWwindow* window, int key, int scancode, int action, int mods);
void framebuffer_size_callback(__attribute__((unused)) GLFWwindow* window, int width, int height);

static ex::App app = ex::App(800, 600, "test");

int main()
{
    ex::set_key_callback(app.window(), key_callbacks);
    app.set_resize_callback(framebuffer_size_callback);

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
    if (action == KEY_PRESS)
    {
        if (key == KEY_ESCAPE)
        {
            app.quit();
        }        
    }
}

void framebuffer_size_callback(__attribute__((unused)) GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
