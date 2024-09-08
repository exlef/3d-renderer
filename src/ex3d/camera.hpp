#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include "transform.hpp"

namespace ex
{
    class Camera
    {
    private:
        glm::vec3 m_forward = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 m_right = glm::vec3(1.0f, 0.0f, 0.0f);
        glm::vec3 m_up = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 m_world_up = glm::vec3(0.0f, 1.0f, 0.0f);

        glm::vec3 m_pos = glm::vec3(0.0, 0.0, 10.0);
        float m_fov = 45;
        float m_near = 0.1f;
        float m_far = 100.0f;
        float m_aspect_ratio = 1;

        // look around
        float m_lastx = 0;
        float m_lasty = 0;
        bool m_at_start = true;
        float cam_speed = 10;
        const float sensitivity = 0.1f;
        float yaw = -90.0;
        float pitch = 0;

        void update_vectors();

    public:
        Transform tr;
        bool is_projection_matrix_require_update = true;

        Camera(float aspect_ratio, glm::vec3 pos = glm::vec3(0.0, 0.0, 10.0), float fov_in_degree = 45, float near = 0.1f, float far = 100.0f);

        void look_around(float xpos, float ypos);

        void move(const App& app);

        void set_fov(float fov_in_degree);

        void set_near(float near);

        void set_far(float far);

        void set_aspect_ratio(float aspect_ratio);

        glm::mat4 get_view_matrix();

        glm::mat4 get_projection_matrix();
    };

    Camera::Camera(float aspect_ratio, glm::vec3 pos, float fov_in_degree, float near, float far)
    {
        m_aspect_ratio = aspect_ratio;
        tr.set_pos(pos);
        // m_pos = pos;
        m_fov = fov_in_degree;
        m_near = near;
        m_far = far;
    }

    void Camera::look_around(float xpos, float ypos)
    {
        if (m_at_start)
        {
            m_lastx = xpos;
            m_lasty = ypos;
            m_at_start = false;
        }

        float xoffset = xpos - m_lastx;
        float yoffset = m_lasty - ypos; // reversed since y-coordinates range from bottom to top
        m_lastx = xpos;
        m_lasty = ypos;

        xoffset *= sensitivity;
        yoffset *= sensitivity;

        yaw += xoffset;
        pitch += yoffset;

        if (pitch > 89.0f)
            pitch = 89.0f;
        if (pitch < -89.0f)
            pitch = -89.0f;

        // tr.set_rot(glm::vec3(yaw, pitch, 0));

        update_vectors();
    }

    // TODO: add abilty to localTranslateXYZ to the transform class.
    // this will probably simplfy camera look around and move logic.
    void Camera::move(const App& app)
    {
        if (ex::is_key_down(app.window(), KEY_E))
        {
            tr.translateY(app.dt() * cam_speed);
        }
        if (ex::is_key_down(app.window(), KEY_Q))
        {
            tr.translateY(app.dt() * -cam_speed);
        }
        if (ex::is_key_down(app.window(), KEY_W))
        {
            tr.translate(m_forward * (cam_speed * app.dt()));
        }
        if (ex::is_key_down(app.window(), KEY_S))
        {
            tr.translate(-m_forward * (cam_speed * app.dt()));
        }
        if (ex::is_key_down(app.window(), KEY_A))
        {
            tr.translate(-m_right * (cam_speed * app.dt()));
        }
        if (ex::is_key_down(app.window(), KEY_D))
        {
            tr.translate(m_right * (cam_speed * app.dt()));
        }
    }

    void Camera::update_vectors()
    {
        // tr.is_dirty = true;

        glm::vec3 direction;

        direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        direction.y = sin(glm::radians(pitch));
        direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));

        m_forward = glm::normalize(direction);
        m_right = glm::normalize(glm::cross(m_forward, m_world_up));
        m_up = glm::normalize(glm::cross(m_right, m_forward));
    }

    void Camera::set_fov(float fov_in_degree)
    {
        m_fov = fov_in_degree;
        is_projection_matrix_require_update = true;
    }

    void Camera::set_near(float near)
    {
        m_near = near;
        is_projection_matrix_require_update = true;
    }

    void Camera::set_far(float far)
    {
        m_far = far;
        is_projection_matrix_require_update = true;
    }

    void Camera::set_aspect_ratio(float aspect_ratio)
    {
        m_aspect_ratio = aspect_ratio;
        is_projection_matrix_require_update = true;
    }

    glm::mat4 Camera::get_view_matrix()
    {
        // tr.is_dirty = false;
        // return glm::lookAt(m_pos, m_pos + m_dir, m_world_up);
        return glm::lookAt(tr.get_pos(), tr.get_pos() + m_forward, m_world_up);
    }

    glm::mat4 Camera::get_projection_matrix()
    {
        is_projection_matrix_require_update = false;
        return glm::perspective(glm::radians(m_fov), m_aspect_ratio, m_near, m_far);
    }

} // namespace ex

