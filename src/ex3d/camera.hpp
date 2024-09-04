#pragma once

#include <glm/gtc/matrix_transform.hpp>

namespace ex
{
    class Camera
    {
    private:
        glm::vec3 m_world_up = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 m_dir = glm::vec3(0.0f, 0.0f, -1.0f);

    public:
        glm::vec3 pos = glm::vec3(0.0, 0.0, 10.0);
        Camera() = default;
        ~Camera() = default;

        glm::mat4 get_view_matrix()
        {
            return glm::lookAt(pos, pos + m_dir, m_world_up);
        }
    };
} // namespace ex

