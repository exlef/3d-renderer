#pragma once

#include "base_shader.hpp"
#include <string>
#include <functional>
#include <memory>

#define GL_SILENCE_DEPRECATION
#include <GL/glew.h>
#include <GLFW/glfw3.h>
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

        bool is_key_down(GLFWwindow* window, u_int32_t key_code);
        
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


#pragma region key_codes

#define KEY_PRESS 1
/* Printable keys */
#define KEY_SPACE 32
#define KEY_APOSTROPHE 39 /* ' */
#define KEY_COMMA 44      /* , */
#define KEY_MINUS 45      /* - */
#define KEY_PERIOD 46     /* . */
#define KEY_SLASH 47      /* / */
#define KEY_0 48
#define KEY_1 49
#define KEY_2 50
#define KEY_3 51
#define KEY_4 52
#define KEY_5 53
#define KEY_6 54
#define KEY_7 55
#define KEY_8 56
#define KEY_9 57
#define KEY_SEMICOLON 59 /* ; */
#define KEY_EQUAL 61     /* = */
#define KEY_A 65
#define KEY_B 66
#define KEY_C 67
#define KEY_D 68
#define KEY_E 69
#define KEY_F 70
#define KEY_G 71
#define KEY_H 72
#define KEY_I 73
#define KEY_J 74
#define KEY_K 75
#define KEY_L 76
#define KEY_M 77
#define KEY_N 78
#define KEY_O 79
#define KEY_P 80
#define KEY_Q 81
#define KEY_R 82
#define KEY_S 83
#define KEY_T 84
#define KEY_U 85
#define KEY_V 86
#define KEY_W 87
#define KEY_X 88
#define KEY_Y 89
#define KEY_Z 90
#define KEY_LEFT_BRACKET 91  /* [ */
#define KEY_BACKSLASH 92     /* \ */
#define KEY_RIGHT_BRACKET 93 /* ] */
#define KEY_GRAVE_ACCENT 96  /* ` */
#define KEY_WORLD_1 161      /* non-US #1 */
#define KEY_WORLD_2 162      /* non-US #2 */

/* Function keys */
#define KEY_ESCAPE 256
#define KEY_ENTER 257
#define KEY_TAB 258
#define KEY_BACKSPACE 259
#define KEY_INSERT 260
#define KEY_DELETE 261
#define KEY_RIGHT 262
#define KEY_LEFT 263
#define KEY_DOWN 264
#define KEY_UP 265
#define KEY_PAGE_UP 266
#define KEY_PAGE_DOWN 267
#define KEY_HOME 268
#define KEY_END 269
#define KEY_CAPS_LOCK 280
#define KEY_SCROLL_LOCK 281
#define KEY_NUM_LOCK 282
#define KEY_PRINT_SCREEN 283
#define KEY_PAUSE 284
#define KEY_F1 290
#define KEY_F2 291
#define KEY_F3 292
#define KEY_F4 293
#define KEY_F5 294
#define KEY_F6 295
#define KEY_F7 296
#define KEY_F8 297
#define KEY_F9 298
#define KEY_F10 299
#define KEY_F11 300
#define KEY_F12 301
#define KEY_F13 302
#define KEY_F14 303
#define KEY_F15 304
#define KEY_F16 305
#define KEY_F17 306
#define KEY_F18 307
#define KEY_F19 308
#define KEY_F20 309
#define KEY_F21 310
#define KEY_F22 311
#define KEY_F23 312
#define KEY_F24 313
#define KEY_F25 314
#define KEY_KP_0 320
#define KEY_KP_1 321
#define KEY_KP_2 322
#define KEY_KP_3 323
#define KEY_KP_4 324
#define KEY_KP_5 325
#define KEY_KP_6 326
#define KEY_KP_7 327
#define KEY_KP_8 328
#define KEY_KP_9 329
#define KEY_KP_DECIMAL 330
#define KEY_KP_DIVIDE 331
#define KEY_KP_MULTIPLY 332
#define KEY_KP_SUBTRACT 333
#define KEY_KP_ADD 334
#define KEY_KP_ENTER 335
#define KEY_KP_EQUAL 336
#define KEY_LEFT_SHIFT 340
#define KEY_LEFT_CONTROL 341
#define KEY_LEFT_ALT 342
#define KEY_LEFT_SUPER 343
#define KEY_RIGHT_SHIFT 344
#define KEY_RIGHT_CONTROL 345
#define KEY_RIGHT_ALT 346
#define KEY_RIGHT_SUPER 347
#define KEY_MENU 348
#pragma endregion
    
} // namespace ex

