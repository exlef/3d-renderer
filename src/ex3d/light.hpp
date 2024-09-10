#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "transform.hpp"

namespace ex
{
    struct DirectionalLight
    {
        ex::Transform tr;
        glm::vec3 color = glm::vec3(0.5f);
    };

    struct PointLight
    {
        ex::Transform tr;
        glm::vec3 color = glm::vec3(1);
        float constant = 1;
        float linear = 0.09f;
        float quadratic = 0.032f;
    };

    class Light
    {
    private:
        /* data */
    public:
        DirectionalLight dir_light;
        PointLight point_light;
        void add_dir_light(glm::vec3 rot, float ambient = 0.5f);
        void add_point_light(glm::vec3 color = glm::vec3(1));
    };

    // ---------------------------------------------------------------------------------------------

    void Light::add_dir_light(glm::vec3 rot, float ambient)
    {
        DirectionalLight l;
        l.tr.set_rot(rot);
        l.color = glm::vec3(ambient);

        dir_light = l;
    }

    void Light::add_point_light(glm::vec3 color)
    {
        PointLight p;
        p.color = color;
        point_light = p;
    }


} // namespace ex

