#pragma once 

#include "ex3d/camera.hpp"

class FlyCam
{
public:
    FlyCam(ex::Camera* cam);
    void look_around(float xpos, float ypos, float dt);
private:
    ex::Camera* m_cam;
    float last_x = 0;
    float last_y = 0;
    bool is_first = true;
    // float cam_speed = 10;
    float sensitivity = 5;
};
