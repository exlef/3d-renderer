#pragma once

#include <glm/glm.hpp>

#include "transform.hpp"

namespace ex 
{
    struct SkyLight
    {
        float strength = 0.2f;
    };

    struct DirectionalLight
    {
        ex::Transform tr;
        glm::vec3 color = glm::vec3(1);
        float strength = 0.5f;
    };

    class PointLight
    {
    public:
        glm::vec3 color = glm::vec3(1);
        float strength = 1;
        float constant = 1;
        float linear = 0.09f;
        float quadratic = 0.032f;

        PointLight(glm::vec3 color);
    };
}