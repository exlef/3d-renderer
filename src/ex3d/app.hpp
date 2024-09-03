#pragma once

#include "GL/glew.h"
#define GL_SILENCE_DEPRECATION
#include "GLFW/glfw3.h"
#include <string>
#include <iostream>


namespace ex
{
    class app
    {
    private:
        int m_screen_width = 0;
        int m_screen_height = 0;
        GLFWwindow* m_window = nullptr;

    public:
        int get_screen_width() const { return m_screen_width; }
        int get_screen_height() const { return m_screen_height; }
        GLFWwindow* window() const { return m_window; }

        int create_window(int width, int height, const std::string& title)
        {
            // glfw: initialize and configure
            // ------------------------------
            glfwInit();
            glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
            glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
            glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
            glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

            // glfw window creation
            // --------------------
            m_screen_width = width;
            m_screen_height = height;
            m_window = glfwCreateWindow(m_screen_width, m_screen_height, title.c_str(), NULL, NULL);
            if (m_window == NULL)
            {
                std::cerr << "Failed to create GLFW window" << std::endl;
                glfwTerminate();
                return -1;
            }
            // this function makes the context of specified window current on the calling thread.
            glfwMakeContextCurrent(m_window);
            // set vsyn on
            glfwSwapInterval(1);
            
            // set callback for when mouse moves
            // glfwSetCursorPosCallback(m_window, mouse_callback);
            // set callback for mouse scroll wheel scrolls
            // glfwSetScrollCallback(m_window, scroll_callback);

            // tell GLFW to capture our mouse
            // glfwSetInputMode(m_window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

            if (glewInit() != GLEW_OK)
            {
                std::cerr << "Failed to initialize GLEW" << std::endl;
                return -1;
            }

            return 1;
        }

        void set_resize_callback(GLFWframebuffersizefun callback)
        {
            glfwSetFramebufferSizeCallback(m_window, callback);
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
            glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);
        }

        void end_drawing()
        {
            glfwSwapBuffers(m_window);
            glfwPollEvents();
        }
    };
} // namespace ex

