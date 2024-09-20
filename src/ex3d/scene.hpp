#pragma once

#include <glm/fwd.hpp>

#include <iostream>
#include <vector>
#include <tuple>

#include "lights.hpp"


namespace ex 
{
    class Scene
    {
    public:
        SkyLight sky_light;
        DirectionalLight dir_light;
        std::vector<std::tuple<glm::vec3, PointLight*>> point_lights;
    };
}
