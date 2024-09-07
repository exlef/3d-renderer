#pragma once

#include <glm/glm.hpp>
#include <vector>

namespace ex
{
    struct DirectionalLight
    {
        glm::vec3 dir;
        glm::vec3 ambient = glm::vec3(0.5f);
        glm::vec3 diffuse = glm::vec3(0.5f);
        glm::vec3 specular = glm::vec3(0.5);
    };
    

    class Light
    {
    private:
        /* data */
    public:
        DirectionalLight dir_light;
        void add_dir_light(glm::vec3 dir, float ambient = 0.5f, float diffuse = 0.5f, float specular = 0.5f);
    };

    // ---------------------------------------------------------------------------------------------

    void Light::add_dir_light(glm::vec3 dir, float ambient, float diffuse, float specular)
    {
        DirectionalLight l;
        l.dir = dir;
        l.ambient = glm::vec3(ambient);
        l.diffuse = glm::vec3(diffuse);
        l.specular = glm::vec3(specular);

        dir_light = l;
    }

} // namespace ex

