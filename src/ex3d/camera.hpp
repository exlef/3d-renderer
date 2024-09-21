#pragma once

#include "transform.hpp"

namespace ex
{
     struct Camera
    {
        Transform tr;
        float aspect_ratio = 1;
        float fov = 45;
        float near = 0.1f;
        float far = 100.0f;
        glm::vec3 world_up = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::mat4 get_view_matrix() const;
        glm::mat4 get_projection_matrix() const;
    };

} // namespace ex

