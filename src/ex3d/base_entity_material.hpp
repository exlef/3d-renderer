#pragma once


#include <iostream>

#include "scene.hpp"

namespace ex 
{
    class BaseEntityMaterial
    {
    public:
        virtual void update(const Scene& scene, std::string entity_id) = 0;
        virtual ~BaseEntityMaterial() = default;
    };
}