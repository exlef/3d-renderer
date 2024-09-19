#pragma once

#include "base_shader.hpp"
#include <string>
#include <functional>
#include <memory>

#define GL_SILENCE_DEPRECATION
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include "input.hpp"
#include "post_processing.hpp"
#include "config.hpp"
#include "skybox.hpp"
#include "entity_manager.hpp"

namespace ex
{
    typedef std::function<void()> update_func;
    typedef std::function<void(int, int)> window_resize_func;
    typedef std::function<void(int, int)> key_callback_func;
    typedef std::function<void(float, float)> mouse_callback_func;

    class App
    {
    public:
        EntityManager entt_man;
        
        // app class stuff TODO: get rid of these. by making game inherit app?
        GLFWwindow* m_window = nullptr;
        update_func m_update = nullptr;
        window_resize_func m_window_resize_funptr = nullptr;
        key_callback_func m_key_callback = nullptr;
        mouse_callback_func m_mouse_callback = nullptr;
        // TODO: implment scene system and add these to the scene
        Camera* cam = nullptr;
        SkyLight sky_light;
        DirectionalLight dir_light;

        App() = delete;
        App(int width, int height, const std::string& title);
        ~App();

        int screen_width() const;
        int screen_height() const;
        float aspect_ratio() const;
        float dt() const;
        GLFWwindow* window() const;
        
        void run();
        void set_window_resize_callback(window_resize_func callback);
        void on_window_resize(int width, int height);
        void set_key_callback(key_callback_func callback);
        void set_mouse_callback(mouse_callback_func callback);
        void set_update_callback(update_func update);
        void quit();
        bool running();
        void start_drawing();
        void draw();
        void end_drawing();
         std::vector<PointLight*> get_point_lights() const;
        void reset_viewport();

    private:
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
        // -------------- TODO: we need these being created after openGL context is created so I will make it pointer to not initialize when app class initialized. is there a better way to handle this?
        // post-processing
        std::unique_ptr<PostProcessing> m_post_processing = nullptr;
        // skybox
        std::unique_ptr<Skybox> m_skybox = nullptr;
    };
    
} // namespace ex

