#include "app.hpp"
#include "glm/fwd.hpp"
#include "helper.hpp"

namespace ex 
{
    static void framebuffer_resize_callback(__attribute__((unused)) GLFWwindow* window, int width, int height);
    static void key_callbacks(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouse_callback(__attribute__((unused)) GLFWwindow* window, double xpos, double ypos);

    App::App(int width, int height, const std::string& title)
        :cam((float)width / (float)height, glm::vec3(0, 2, 10))
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
        window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
        if (window == NULL)
        {
            std::cerr << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
        }
        // this function makes the context of specified window current on the calling thread.
        glfwMakeContextCurrent(window);
        // set vsyn on
        glfwSwapInterval(1);
        // Set the user pointer to the current instance of App
        glfwSetWindowUserPointer(window, this);
        // hide cursor
        if(hide_cursor) glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        if (glewInit() != GLEW_OK)
        {
            std::cerr << "Failed to initialize GLEW" << std::endl;
            glfwTerminate();
        }
        // set_callbacks
        glfwSetFramebufferSizeCallback(window, framebuffer_resize_callback);
        glfwSetKeyCallback(window, key_callbacks);
        glfwSetCursorPosCallback(window, mouse_callback);

        // configure global opengl states
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_CULL_FACE);        
        
        // implement a macro that checks if openGL context has been created and put it for constructors of this things that need openGL context being present
        post_processing = std::make_unique<PostProcessing>(get_framebuffer_dimensions());
        m_skybox = std::make_unique<Skybox>();
    }
    App::~App(){ glfwTerminate(); }

    void App::run()
    {
        while (!glfwWindowShouldClose(window))
        {
            auto frameStart = std::chrono::high_resolution_clock::now();

            frameCount++;

            // Calculate elapsed time
            auto currentTime = std::chrono::high_resolution_clock::now();
            elapsedTime += std::chrono::duration<double>(currentTime - lastTime).count();
            dt = std::chrono::duration<double>(currentTime - lastTime).count();
            lastTime = currentTime;

            update_scene();

            start_drawing();

            on_update();

            draw();

            end_drawing();

            // Update the window title every second
            if (elapsedTime >= 1.0)
            {
                // Calculate FPS
                double fps = frameCount / elapsedTime;

                // Create a title string
                std::string title = "FPS: " + std::to_string(static_cast<int>(fps));
                glfwSetWindowTitle(window, title.c_str());

                // Reset frame count and elapsed time
                frameCount = 0;
                elapsedTime = 0.0;
            }

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

    void App::update_scene()
    {
        scene.sky_light = sky_light;
        scene.dir_light = dir_light;
        
        scene.point_lights.clear();
        for(auto& e : entt_man.entities)
        {
            if(e.point_light && e.tr)
            {
                // complete this line to add point lights tho scene
                scene.point_lights.emplace_back(e.tr->pos, e.point_light.get());
            }                
        }

        scene.camera = &cam;
    }

    void App::start_drawing()
    {
        if(apply_pp)
        {
            // Bind the custom framebuffer
            glBindFramebuffer(GL_FRAMEBUFFER, post_processing->FBO);
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

    void App::draw()
    {
        // for(auto& e : entt_man.entities)
        // {
        //     if(e.shader && e.mesh && e.tr)
        //     {
        //         e.shader->update();
        //         for (unsigned int i = 0; i < e.mesh->meshes.size(); i++)
        //         {
        //             e.mesh->meshes[i].Draw();
        //         }
        //     }                
        // }
    }

    void App::end_drawing()
    {
        

        // draw skybox as last
        glc(glDepthFunc(GL_LEQUAL)); // change depth function so depth test passes when values are equal to depth buffer's content
        m_skybox->update_shader(&cam);
        glc(glBindVertexArray(m_skybox->skyboxVAO));
        glc(glActiveTexture(GL_TEXTURE0));
        glc(glBindTexture(GL_TEXTURE_CUBE_MAP, m_skybox->cubemapTexture));
        glc(glDrawArrays(GL_TRIANGLES, 0, 36));
        glc(glBindVertexArray(0));
        glc(glDepthFunc(GL_LESS)); // set depth function back to default

        if (apply_pp)
        {
            // Bind the default framebuffer
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            // Draw the framebuffer rectangle

            post_processing->shader.use();
            glBindVertexArray(post_processing->rectVAO);
            glDisable(GL_DEPTH_TEST); // prevents framebuffer rectangle from being discarded
            glBindTexture(GL_TEXTURE_2D, post_processing->framebufferTexture);
            glDrawArrays(GL_TRIANGLES, 0, 6);
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    void App::quit()
    {
        glfwSetWindowShouldClose(window, true);
    }


    glm::vec2 App::get_framebuffer_dimensions() const
    {
        int width, height;
        glfwGetFramebufferSize(window, &width, &height);
        return glm::vec2(width, height);
    }

    float App::get_aspect_ratio() const 
    {
        glm::vec2 dimensions = get_framebuffer_dimensions();
        return dimensions.x / dimensions.y;
    }

    std::vector<PointLight*> App::get_point_lights() const
    {
        std::vector<PointLight*> lights;
        for(auto& e : entt_man.entities)
        {
            if(e.point_light && e.tr)
            {
                PointLight* l =  e.point_light.get();
                lights.push_back(l);
            }                
        }
        return lights;
    }

    bool App::is_key_down(u_int32_t key_code){ return glfwGetKey(window, key_code) == KEY_PRESS; }

    static void framebuffer_resize_callback(__attribute__((unused)) GLFWwindow* window, int width, int height)
    {
        // Retrieve the App instance from the user pointer
        App* app = static_cast<App*>(glfwGetWindowUserPointer(window));
        if (app)
        {
            app->post_processing = std::make_unique<PostProcessing>(glm::vec2(width, height));
            glViewport(0, 0, width, height);
            app->on_framebuffer_resize(width, height);
        }
    }
    static void key_callbacks(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        UNUSED(scancode);
        UNUSED(mods);

        // Retrieve the App instance from the user pointer
        App* app = static_cast<App*>(glfwGetWindowUserPointer(window));
        if (app)
        {
            app->on_key_callbacks(key, action);
        }
    }
    static void mouse_callback(GLFWwindow* window, double xpos, double ypos)
    {
        UNUSED(window);
        App* app = static_cast<App*>(glfwGetWindowUserPointer(window));
        if (app)
        {
            app->on_mouse_move(xpos, ypos);
        }
    }
}