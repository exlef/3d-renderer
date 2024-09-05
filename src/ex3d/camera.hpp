#pragma once

#include <glm/gtc/matrix_transform.hpp>

namespace ex
{
    class Camera
    {
    private:
        glm::vec3 m_dir = glm::vec3(0.0f, 0.0f, -1.0f);
        glm::vec3 m_world_up = glm::vec3(0.0f, 1.0f, 0.0f);

    public:
        glm::vec3 pos = glm::vec3(0.0, 0.0, 10.0);
        float fov = 45;
        float near = 0.1f;
        float far = 100.0f;

        Camera(glm::vec3 pos = glm::vec3(0.0, 0.0, 10.0), float fov = 45, float near = 0.1f, float far = 100.0f);
        ~Camera() = default;

        float fov_in_rad()
        {
            return glm::radians(fov);
        }

        glm::mat4 get_view_matrix()
        {
            return glm::lookAt(pos, pos + m_dir, m_world_up);
        }

        glm::mat4 get_projection_matrix(float screen_ratio)
        {
            return glm::perspective(fov_in_rad(), screen_ratio, near, far);
        }
    };

    Camera::Camera(glm::vec3 pos, float fov, float near, float far)
    {
        this->pos = pos;
        this->fov = fov;
        this->near = near;
        this->far = far;
    }
} // namespace ex

