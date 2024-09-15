#pragma once

#include <GL/glew.h>
#define GL_SILENCE_DEPRECATION
#include <GLFW/glfw3.h>
#include <iostream>
#include <string>
#include <functional>
#include <memory>

#include "input.hpp"
#include "mesh.hpp"
#include "model.hpp"
#include "post_processing.hpp"
#include "config.hpp"
#include "camera.hpp"
#include "skybox.hpp"

namespace ex
{
    typedef std::function<void()> update_func;
    typedef std::function<void(int, int)> window_resize_func;
    typedef std::function<void(int, int)> key_callback_func;
    typedef std::function<void(float, float)> mouse_callback_func;

    class App
    {
    private:
        GLFWwindow* m_window = nullptr;
        update_func m_update = nullptr;
        window_resize_func m_window_resize_funptr = nullptr;
        key_callback_func m_key_callback = nullptr;
        mouse_callback_func m_mouse_callback = nullptr;

        // config
        bool hide_cursor = true;
        bool apply_pp = APPLY_GAMMA_CRRC; // apply post processing

        // delta time
        const int TARGET_FPS = 60;
        const double TARGET_FRAME_TIME = 1.0 / TARGET_FPS;
        int frameCount = 0;
        double elapsedTime = 0.0;
        std::chrono::high_resolution_clock::time_point lastTime = std::chrono::high_resolution_clock::now();
        float m_dt = 0;

        // post-processing
        std::unique_ptr<PostProcessing> m_post_processing = nullptr;

        // skybox
        Skybox skybox;

    public:
        Camera* cam = nullptr;
        int screen_width() const 
        {
            int width, height;
            glfwGetFramebufferSize(m_window, &width, &height);
            return width;
        }
        int screen_height() const 
        {
            int width, height;
            glfwGetFramebufferSize(m_window, &width, &height);
            return height;
        }
        float aspect_ratio() const 
        {
            int width, height;

            glfwGetFramebufferSize(m_window, &width, &height);
            return (float)width / (float)height;
        }
        float dt() const { return m_dt; }
        GLFWwindow* window() const { return m_window; }
        App() = delete;

        App(int width, int height, const std::string& title)
        {
            // glfw: initialize and configure
            glfwInit();
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

            // glfw window creation
            m_window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
            if (m_window == NULL)
            {
                std::cerr << "Failed to create GLFW window" << std::endl;
                glfwTerminate();
            }
            // this function makes the context of specified window current on the calling thread.
            glfwMakeContextCurrent(m_window);
            // set vsyn on
            glfwSwapInterval(1);
            // Set the user pointer to the current instance of App
            glfwSetWindowUserPointer(window(), this);
            // hide cursor
            if(hide_cursor) glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

            if (glewInit() != GLEW_OK)
            {
                std::cerr << "Failed to initialize GLEW" << std::endl;
                glfwTerminate();
            }

            // configure global opengl states
            glEnable(GL_DEPTH_TEST);
            glEnable(GL_CULL_FACE);
            // glCullFace(GL_BACK);
            // glFrontFace(GL_CCW);
            // glEnable(GL_FRAMEBUFFER_SRGB);
            
            // this object needs to be created after the openGL contex because the shader object of post processing depends on openGL being initialized. creating this post processing object before openGL being initialized will cause seg fault.
            // we can't simply give width and height for post prcessing texture size. it will be stretched. it need to be framebuffer size.
            int framebufferWidth, framebufferHeight;
            glfwGetFramebufferSize(m_window, &framebufferWidth, &framebufferHeight);
            m_post_processing = std::make_unique<PostProcessing>(framebufferWidth, framebufferHeight);
        }

        ~App()
        {
            // This will clean up the window and other GLFW resources
            glfwTerminate();
        }

        void run()
        {
            while (running())
            {
                auto frameStart = std::chrono::high_resolution_clock::now();

                frameCount++;

                // Calculate elapsed time
                auto currentTime = std::chrono::high_resolution_clock::now();
                elapsedTime += std::chrono::duration<double>(currentTime - lastTime).count();
                m_dt = std::chrono::duration<double>(currentTime - lastTime).count();
                lastTime = currentTime;

                start_drawing();

                m_update();

                end_drawing();

                // Update the window title every second
                if (elapsedTime >= 1.0)
                {
                    // Calculate FPS
                    double fps = frameCount / elapsedTime;

                    // Create a title string
                    std::string title = "FPS: " + std::to_string(static_cast<int>(fps));
                    glfwSetWindowTitle(window(), title.c_str());

                    // Reset frame count and elapsed time
                    frameCount = 0;
                    elapsedTime = 0.0;
                }

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

        void set_window_resize_callback(window_resize_func callback)
        {
            m_window_resize_funptr = callback;
            glfwSetFramebufferSizeCallback(m_window, framebuffer_size_callback);
        }

        static void framebuffer_size_callback(__attribute__((unused)) GLFWwindow* window, int width, int height)
        {
            // Retrieve the App instance from the user pointer
            App* app = static_cast<App*>(glfwGetWindowUserPointer(window));
            if (app)
            {
                if(app->m_window_resize_funptr)
                {
                    app->m_window_resize_funptr(width, height);
                }
                app->on_window_resize(width, height);
                glViewport(0, 0, width, height);
            }
        }

        void on_window_resize(int width, int height)
        {
            // m_post_processing = new PostProcessing(width, height);
            // in here we don't need glfwGetFramebufferSize as we did in the constructor. because the widht and height is already framebuffer sizes.
            m_post_processing = std::make_unique<PostProcessing>(width, height);
        }

        void set_key_callback(key_callback_func callback)
        {
            m_key_callback = callback;
            glfwSetKeyCallback(window(), key_callbacks);
        }

        static void key_callbacks(GLFWwindow* window, int key, __attribute__((unused)) int scancode, int action, __attribute__((unused)) int mods)
        {
            // Retrieve the App instance from the user pointer
            App* app = static_cast<App*>(glfwGetWindowUserPointer(window));
            if (app)
            {
                app->m_key_callback(key, action);
            }
        }

        void set_mouse_callback(mouse_callback_func callback)
        {
            m_mouse_callback = callback;
            glfwSetCursorPosCallback(m_window, mouse_callback);
        }

        static void mouse_callback(__attribute__((unused)) GLFWwindow* window, double xpos, double ypos)
        {
            App* app = static_cast<App*>(glfwGetWindowUserPointer(window));
            if (app)
            {
                app->m_mouse_callback((float)xpos, (float)ypos);
            }
        }

        void set_update_callback(update_func update)
        {
            m_update = update;
        }

        void quit()
        {
            glfwSetWindowShouldClose(m_window, true);
        }

        bool running()
        {
            return !glfwWindowShouldClose(m_window);
        }

        void start_drawing()
        {
            if(apply_pp)
            {
                // Bind the custom framebuffer
                glBindFramebuffer(GL_FRAMEBUFFER, m_post_processing->FBO);
                // Specify the color of the background
                float gamma = 2.2; // TODO: this should be stored somewhere and send to the post processing fragment shader
                glClearColor(pow(0.07f, gamma), pow(0.13f, gamma), pow(0.17f, gamma), 1.0f); // if the post processing is enabled I want to background color also be effected from gamma correction. since we can't set it from post processing fragment shader I set it here.
                // Enable depth testing since it's disabled when drawing the framebuffer rectangle
                glEnable(GL_DEPTH_TEST);
            }
            else 
            {
                glClearColor(0.9f, 0.2f, 0.2f, 1.0f);
            }
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        }

        void draw(Model& model)
        {
            for (unsigned int i = 0; i < model.meshes.size(); i++)
                model.meshes[i].Draw();
        }

        void end_drawing()
        {
            

            // draw skybox as last
            glc(glDepthFunc(GL_LEQUAL)); // change depth function so depth test passes when values are equal to depth buffer's content
            skybox.update_shader(cam);
            glc(glBindVertexArray(skybox.skyboxVAO));
            glc(glActiveTexture(GL_TEXTURE0));
            glc(glBindTexture(GL_TEXTURE_CUBE_MAP, skybox.cubemapTexture));
            glc(glDrawArrays(GL_TRIANGLES, 0, 36));
            glc(glBindVertexArray(0));
            glc(glDepthFunc(GL_LESS)); // set depth function back to default

            if (apply_pp)
            {
                // Bind the default framebuffer
                glBindFramebuffer(GL_FRAMEBUFFER, 0);
                // Draw the framebuffer rectangle

                m_post_processing->pp_shader_prog.use();
                glBindVertexArray(m_post_processing->rectVAO);
                glDisable(GL_DEPTH_TEST); // prevents framebuffer rectangle from being discarded
                glBindTexture(GL_TEXTURE_2D, m_post_processing->framebufferTexture);
                glDrawArrays(GL_TRIANGLES, 0, 6);
            }

            glfwSwapBuffers(m_window);
            glfwPollEvents();
        }
    };
    
} // namespace ex

