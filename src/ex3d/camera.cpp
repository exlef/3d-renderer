#include "camera.hpp"


namespace ex 
{
    Camera::Camera(float aspect_ratio, glm::vec3 pos, float fov_in_degree, float near, float far)
    {
        this->aspect_ratio = aspect_ratio;
        tr.pos = pos;
        m_fov = fov_in_degree;
        m_near = near;
        m_far = far;
    }

    glm::mat4 Camera::get_view_matrix() const
    {
        return glm::lookAt(tr.pos, tr.pos + tr.get_forward(), m_world_up);
    }

    glm::mat4 Camera::get_projection_matrix() const
    {
        return glm::perspective(glm::radians(m_fov), aspect_ratio, m_near, m_far);
    }
}