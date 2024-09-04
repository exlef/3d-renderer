#include "ex3d/app.hpp"
#include "ex3d/input.hpp"
#include <thread>

void key_callbacks(GLFWwindow* window, int key, int scancode, int action, int mods);
void framebuffer_size_callback(__attribute__((unused)) GLFWwindow* window, int width, int height);



int main()
{
    ex::App app = ex::App(800, 600, "test");

    // ex::set_key_callback(app.window(), key_callbacks);
    // app.set_resize_callback(framebuffer_size_callback);

    const int TARGET_FPS = 60;
    const double TARGET_FRAME_TIME = 1.0 / TARGET_FPS;
    std::cout << TARGET_FRAME_TIME << std::endl;
    int frameCount = 0;
    double elapsedTime = 0.0;
    auto lastTime = std::chrono::high_resolution_clock::now();

    while (app.running())
    {
        auto frameStart = std::chrono::high_resolution_clock::now();

        frameCount++;

        // Calculate elapsed time
        auto currentTime = std::chrono::high_resolution_clock::now();
        elapsedTime += std::chrono::duration<double>(currentTime - lastTime).count();
        lastTime = currentTime;

        // Update the window title every second
        if (elapsedTime >= 1.0)
        {
            // Calculate FPS
            double fps = frameCount / elapsedTime;

            // Create a title string
            std::string title = "FPS: " + std::to_string(static_cast<int>(fps));
            glfwSetWindowTitle(app.window(), title.c_str());
            std::cout << "hello" << "\n";

            // Reset frame count and elapsed time
            frameCount = 0;
            elapsedTime = 0.0;
        }

        app.start_drawing();

        if (ex::is_key_down(app.window(), KEY_W))
        {
            std::cout << "W is being pressed" << std::endl;
        }

        app.end_drawing();

        // Wait for the remaining time to achieve the target frame time
        // auto frameEnd = std::chrono::high_resolution_clock::now();
        // std::chrono::duration<double> frameDuration = frameEnd - frameStart;
        // double sleepTime = TARGET_FRAME_TIME - frameDuration.count();
        // if (sleepTime > 0)
        // {
        //     std::this_thread::sleep_for(std::chrono::duration<double>(sleepTime));
        // }

        // Busy-wait loop to achieve the target frame time
        auto frameEnd = std::chrono::high_resolution_clock::now();
        std::chrono::duration<double> frameDuration = frameEnd - frameStart;
        double sleepTime = TARGET_FRAME_TIME - frameDuration.count();
        while (sleepTime > 0)
        {
            frameEnd = std::chrono::high_resolution_clock::now();
            frameDuration = frameEnd - frameStart;
            sleepTime = TARGET_FRAME_TIME - frameDuration.count();
        }
    }
}

void key_callbacks(__attribute__((unused)) GLFWwindow* window, int key, __attribute__((unused)) int scancode, int action, __attribute__((unused)) int mods)
{
    if (action == KEY_PRESS)
    {
        if (key == KEY_ESCAPE)
        {
            // app.quit();
        }        
    }
}

void framebuffer_size_callback(__attribute__((unused)) GLFWwindow* window, int width, int height)
{
    glViewport(0, 0, width, height);
}
