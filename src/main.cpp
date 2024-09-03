#include "ex3d/renderer.hpp"

void quit(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
    {
        std::cout << "escape pressed" << std::endl;
    }
}

int main()
{
    ex::renderer renderer;
    renderer.create_window(800, 600, "test");

    ex::set_key_callback(renderer.window(), quit);

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
