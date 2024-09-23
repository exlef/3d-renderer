#include "app.hpp"
#include "config.hpp"
#include "glm/fwd.hpp"
#include "helper.hpp"

namespace ex 
{
    static void framebuffer_resize_callback(__attribute__((unused)) GLFWwindow* window, int width, int height);
    static void key_callbacks(GLFWwindow* window, int key, int scancode, int action, int mods);
    static void mouse_callback(__attribute__((unused)) GLFWwindow* window, double xpos, double ypos);

    App::App(int width, int height, const std::string& title) : scene((float)width/(float)height)
    {
        // glfw: initialize and configure
        glfwInit();
        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

        window = glfwCreateWindow(width, height, title.c_str(), NULL, NULL);
        if (window == NULL)
        {
            std::cerr << "Failed to create GLFW window" << std::endl;
            glfwTerminate();
        }
        
        glfwMakeContextCurrent(window);// this function makes the context of specified window current on the calling thread.
        glfwSwapInterval(1); // set vsyn on
        glfwSetWindowUserPointer(window, this);// Set the user pointer to the current instance of App
        if(HIDE_CURSOR) glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

        if (glewInit() != GLEW_OK)
        {
            std::cerr << "Failed to initialize GLEW" << std::endl;
            glfwTerminate();
        }
        // set_callbacks
        glfwSetFramebufferSizeCallback(window, framebuffer_resize_callback);
        glfwSetKeyCallback(window, key_callbacks);
        glfwSetCursorPosCallback(window, mouse_callback);

        glClearColor(CLEAR_COLOR.r, CLEAR_COLOR.g, CLEAR_COLOR.b, CLEAR_COLOR.a);

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
        scene.point_lights.clear();
        for(auto& e : entt_man.entities)
        {
            if(e.point_light && e.tr)
            {
                scene.point_lights.emplace_back(e.tr->pos, e.point_light.get());
            }                
        }
    }

    void App::start_drawing()
    {
        static bool x= true;
        if(x)
        {
            x = false;
            #pragma region shadow
            // store dimesnsions since we want to reset back to it.
            glm::vec2 dims = get_framebuffer_dimensions();

            unsigned int depthMapFBO;
            glGenFramebuffers(1, &depthMapFBO);
            const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

            unsigned int depthMap;
            glGenTextures(1, &depthMap);
            glBindTexture(GL_TEXTURE_2D, depthMap);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH_COMPONENT, 
            SHADOW_WIDTH, SHADOW_HEIGHT, 0, GL_DEPTH_COMPONENT, GL_FLOAT, NULL);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT); 
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

            glBindFramebuffer(GL_FRAMEBUFFER, depthMapFBO);
            glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_TEXTURE_2D, depthMap, 0);
            glDrawBuffer(GL_NONE);
            glReadBuffer(GL_NONE);

            std::cout << depthMap << std::endl;
            
            float near_plane = 1.0f, far_plane = 7.5f;
            glm::mat4 lightProjection = glm::ortho(-10.0f, 10.0f, -10.0f, 10.0f, near_plane, far_plane); 
            glm::mat4 lightView = glm::lookAt(glm::vec3(-2.0f, 4.0f, -1.0f), 
                                    glm::vec3( 0.0f, 0.0f,  0.0f), 
                                    glm::vec3( 0.0f, 1.0f,  0.0f));
            glm::mat4 lightSpaceMatrix = lightProjection * lightView;  

            Shader depth_shader;
            depth_shader.create_shader_program("src/ex3d/shaders/shadow_map.vert", "src/ex3d/shaders/shadow_map.frag");
            depth_shader.use();
            depth_shader.setMat4("lightSpaceMatrix", lightSpaceMatrix);
            
            glViewport(0, 0, SHADOW_WIDTH, SHADOW_HEIGHT);
            glClear(GL_DEPTH_BUFFER_BIT);
            for(auto& e : entt_man.entities)
            {
                if(e.material && e.mesh && e.tr)
                {
                    depth_shader.setMat4("model", e.tr->get_model_matrix());
                    for (unsigned int i = 0; i < e.mesh->meshes.size(); i++)
                    {
                        e.mesh->meshes[i].Draw();
                    }
                }                
            }
            // reset
            glViewport(0, 0, dims.x, dims.y);
            glBindFramebuffer(GL_FRAMEBUFFER, 0);
            #pragma endregion
        }

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // needed?
        if(APPLY_PP) { post_processing->prepare(); }
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }

    void App::draw()
    {
        for(auto& e : entt_man.entities)
        {
            if(e.material && e.mesh && e.tr)
            {
                e.material->update(scene, e.id());
                for (unsigned int i = 0; i < e.mesh->meshes.size(); i++)
                {
                    e.mesh->meshes[i].Draw();
                }
            }                
        }
    }

    void App::end_drawing()
    {
        if(SHOW_SKYBOX) m_skybox->draw(&scene.camera);
        if (APPLY_PP) { post_processing->draw(); }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
#pragma region helper functions, getters and input
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

    float App::get_time() const
    {
        return glfwGetTime();
    }

    bool App::is_key_down(u_int32_t key_code){ return glfwGetKey(window, key_code) == KEY_PRESS; }
#pragma endregion

#pragma region callbacks
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
#pragma endregion
}