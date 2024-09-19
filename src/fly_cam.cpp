#include "fly_cam.hpp"

FlyCam::FlyCam(ex::Camera* cam)
{
    m_cam = cam;
}

void FlyCam::look_around(float xpos, float ypos, float dt)
{
    if (is_first)
    {
        last_x = xpos;
        last_y = ypos;
        is_first = false;
    }
    float dx = (last_x - xpos) * sensitivity;
    float dy = (last_y - ypos) * sensitivity;
    last_x = xpos;
    last_y = ypos;
    m_cam->tr.local_rotateY(dx * dt);
    m_cam->tr.local_rotateX(dy * dt);
}