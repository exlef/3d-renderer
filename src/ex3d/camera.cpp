#include "camera.hpp"


namespace ex 
{
    glm::mat4 Camera::get_view_matrix() const
    {
        return glm::lookAt(tr.pos, tr.pos + tr.get_forward(), world_up);
    }

    glm::mat4 Camera::get_projection_matrix() const
    {
        return glm::perspective(glm::radians(fov), aspect_ratio, near, far);
    }
}