#include "ex3d/renderer.hpp"
#include "ex3d/input.hpp"

void key_callbacks(GLFWwindow* window, int key, int scancode, int action, int mods);

int main()
{
    ex::renderer renderer;
    renderer.create_window(800, 600, "test");

    ex::set_key_callback(renderer.window(), key_callbacks);

    while (renderer.running())
    {
        renderer.start_drawing();
        
        if(ex::is_key_down(renderer.window(), KEY_W))
        {
            std::cout << "W is being pressed" << std::endl;
        }
        

        renderer.end_drawing();
    }
}

void key_callbacks(__attribute__((unused)) GLFWwindow* window, int key, __attribute__((unused)) int scancode, int action, __attribute__((unused)) int mods)
{
    if (action == GLFW_PRESS)
    {
        if (key == GLFW_KEY_ESCAPE)
        {
            std::cout << "escape pressed" << std::endl;
        }
    }

    if (action == GLFW_RELEASE)
    {
        if (key == GLFW_KEY_ESCAPE)
        {
            std::cout << "escape released" << std::endl;
        }
    }
}
