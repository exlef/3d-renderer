#pragma once

#include <glm/gtc/matrix_transform.hpp>
#include "transform.hpp"

namespace ex
{
    class Camera
    {
    private:
        glm::vec3 m_dir = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 m_world_up = glm::vec3(0.0f, 1.0f, 0.0f);

        glm::vec3 m_pos = glm::vec3(0.0, 0.0, 10.0);
        float m_fov = 45;
        float m_near = 0.1f;
        float m_far = 100.0f;
        float m_aspect_ratio = 1;

        float fov_in_rad()
        {
            return glm::radians(m_fov);
        }

    public:
        Transform tr;
        bool is_projection_matrix_require_update = true;

        Camera(float aspect_ratio, glm::vec3 pos = glm::vec3(0.0, 0.0, 10.0), float fov_in_degree = 45, float near = 0.1f, float far = 100.0f);
        ~Camera() = default;

        void set_fov(float fov_in_degree)
        {
            m_fov = fov_in_degree;
            is_projection_matrix_require_update = true;
        }

        void set_near(float near)
        {
            m_near = near;
            is_projection_matrix_require_update = true;
        }

        void set_far(float far)
        {
            m_far = far;
            is_projection_matrix_require_update = true;
        }

        void set_aspect_ratio(float aspect_ratio)
        {
            m_aspect_ratio = aspect_ratio;
            is_projection_matrix_require_update = true;
        }

        glm::mat4 get_view_matrix()
        {
            tr.is_dirty = false;
            return glm::lookAt(m_pos, m_pos + m_dir, m_world_up);
        }

        glm::mat4 get_projection_matrix()
        {
            is_projection_matrix_require_update = false;
            return glm::perspective(fov_in_rad(), m_aspect_ratio, m_near, m_far);
        }

    };

    Camera::Camera(float aspect_ratio, glm::vec3 pos, float fov_in_degree, float near, float far)
    {
        m_aspect_ratio = aspect_ratio;
        m_pos = pos;
        m_fov = fov_in_degree;
        m_near = near;
        m_far = far;
    }
} // namespace ex

