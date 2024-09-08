#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include "transform.hpp"

namespace ex
{
    class Camera
    {
    private:
        glm::vec3 m_world_up = glm::vec3(0.0f, 1.0f, 0.0f);

        glm::vec3 m_pos = glm::vec3(0.0, 0.0, 10.0);
        float m_fov = 45;
        float m_near = 0.1f;
        float m_far = 100.0f;
        float m_aspect_ratio = 1;

    public:
        Transform tr;
        bool is_projection_matrix_require_update = true;

        Camera(float aspect_ratio, glm::vec3 pos = glm::vec3(0.0, 0.0, 10.0), float fov_in_degree = 45, float near = 0.1f, float far = 100.0f);

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
        m_fov = fov_in_degree;
        m_near = near;
        m_far = far;
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
        return glm::lookAt(tr.get_pos(), tr.get_pos() + tr.get_forward(), m_world_up);
    }

    glm::mat4 Camera::get_projection_matrix()
    {
        is_projection_matrix_require_update = false;
        return glm::perspective(glm::radians(m_fov), m_aspect_ratio, m_near, m_far);
    }

} // namespace ex

