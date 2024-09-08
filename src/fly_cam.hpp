#pragma once 

#include "ex3d/camera.hpp"
#include "ex3d/input.hpp"
#include "ex3d/app.hpp"

class FlyCam
{
private:
    ex::Camera* m_cam;

    float last_x = 0;
    float last_y = 0;
    bool is_first = true;
    float cam_speed = 10;
    float sensitivity = 5;


public:
    FlyCam(ex::Camera* cam);
    ~FlyCam();

    void look_around(float xpos, float ypos, float dt);
    void move(const ex::App& app);
};

FlyCam::FlyCam(ex::Camera* cam)
{
    m_cam = cam;
}

FlyCam::~FlyCam()
{
}

void FlyCam::move(const ex::App& app)
{
    if (ex::is_key_down(app.window(), KEY_E))
    {
        m_cam->tr.translateY(app.dt() * cam_speed);
        // tr.local_translateY(app.dt() * cam_speed);
    }
    if (ex::is_key_down(app.window(), KEY_Q))
    {
        m_cam->tr.translateY(app.dt() * -cam_speed);
        // tr.local_translateY(app.dt() * -cam_speed);
    }
    if (ex::is_key_down(app.window(), KEY_W))
    {
        // tr.translate(m_forward * (cam_speed * app.dt()));
        m_cam->tr.local_translateZ(app.dt() * cam_speed);
    }
    if (ex::is_key_down(app.window(), KEY_S))
    {
        // tr.translate(-m_forward * (cam_speed * app.dt()));
        m_cam->tr.local_translateZ(app.dt() * -cam_speed);
    }
    if (ex::is_key_down(app.window(), KEY_A))
    {
        // tr.translate(-m_right * (cam_speed * app.dt()));
        m_cam->tr.local_translateX(app.dt() * -cam_speed);
    }
    if (ex::is_key_down(app.window(), KEY_D))
    {
        // tr.translate(m_right * (cam_speed * app.dt()));
        m_cam->tr.local_translateX(app.dt() * cam_speed);
    }
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
