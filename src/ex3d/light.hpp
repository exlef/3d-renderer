#pragma once

#include <glm/glm.hpp>
#include <vector>

#include "transform.hpp"

namespace ex
{
    struct DirectionalLight
    {
        // glm::vec3 dir = glm::vec3(-2.0f);
        ex::Transform tr;
        glm::vec3 color = glm::vec3(0.5f);
        
    };
    

    class Light
    {
    private:
        /* data */
    public:
        DirectionalLight dir_light;
        void add_dir_light(glm::vec3 rot, float ambient = 0.5f);
    };

    // ---------------------------------------------------------------------------------------------

    void Light::add_dir_light(glm::vec3 rot, float ambient)
    {
        DirectionalLight l;
        // l.dir = dir;
        l.tr.set_rot(rot.x, rot.y, rot.z);
        l.color = glm::vec3(ambient);

        dir_light = l;
    }

} // namespace ex

