#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "transform.hpp"

namespace ex
{
    struct SkyLight
    {
        float strength = 0;
    };

    struct DirectionalLight
    {
        ex::Transform tr;
        glm::vec3 color = glm::vec3(0);
        float strength = 0;
    };

    struct PointLight
    {
        ex::Transform tr;
        glm::vec3 color = glm::vec3(1);
        float strength = 0;

        float constant = 1;
        float linear = 0.09f;
        float quadratic = 0.032f;
    };

    class Light
    {
    private:
        /* data */
    public:
        SkyLight sky_light;
        DirectionalLight dir_light;
        std::vector<PointLight> point_lights;
        void add_sky_light(float strength);
        void add_dir_light(glm::vec3 rot, float strength = 1);
        void add_point_light(glm::vec3 pos, glm::vec3 color = glm::vec3(1), float strength = 1);
    };

    // ---------------------------------------------------------------------------------------------
    void Light::add_sky_light(float strength)
    {
        SkyLight sl;
        sl.strength = strength;
        sky_light = sl;
    }

    void Light::add_dir_light(glm::vec3 rot, float strength)
    {
        DirectionalLight l;
        l.tr.set_rot(rot);
        dir_light.strength = strength;

        dir_light = l;
    }

    void Light::add_point_light(glm::vec3 pos, glm::vec3 color, float strength)
    {
        PointLight p;
        p.color = color;
        p.strength = strength;
        p.tr.pos = pos;
        // TODO: avoid unnecesary copying and create struct directly in the vector (emplace_back)
        point_lights.push_back(p);
    }


} // namespace ex

