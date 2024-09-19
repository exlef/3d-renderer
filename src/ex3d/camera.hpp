#pragma once

#include "transform.hpp"

namespace ex
{
    class Camera
    {
    public:
        float aspect_ratio = 1;
        Transform tr;

        Camera(float aspect_ratio, glm::vec3 pos = glm::vec3(0.0, 0.0, 10.0), float fov_in_degree = 45, float near = 0.1f, float far = 100.0f);

        glm::mat4 get_view_matrix() const;

        glm::mat4 get_projection_matrix() const;

    private:
        glm::vec3 m_world_up = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 m_pos = glm::vec3(0.0, 0.0, 10.0);
        float m_fov = 45;
        float m_near = 0.1f;
        float m_far = 100.0f;
    };

} // namespace ex

