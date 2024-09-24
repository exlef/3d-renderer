#pragma once

#include <glm/fwd.hpp>

#include <iostream>
#include <vector>
#include <tuple>

#include "lights.hpp"
#include "camera.hpp"


namespace ex 
{
    class Scene
    {
    public:
        SkyLight sky_light;
        DirectionalLight dir_light;
        std::vector<std::tuple<glm::vec3, PointLight*>> point_lights;
        Camera camera;
        glm::mat4 lightSpaceMatrix;
        unsigned int shadow_map_texture_id;
        glm::vec3 lightPos;

        Scene(float aspect_ration);
    };
}
