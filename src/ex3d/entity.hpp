#pragma once

#include <memory>

#include "transform.hpp"
#include "mesh_comp.hpp"
#include "lights.hpp"
#include "base_entity_material.hpp"


namespace ex
{
    class Entity
    {
    public:
        Entity(std::string id);
        std::string id() const;
        // components
        std::unique_ptr<Transform> tr = nullptr;
        std::unique_ptr<MeshComponent> mesh = nullptr;
        std::unique_ptr<BaseEntityMaterial> material = nullptr;
        std::unique_ptr<PointLight> point_light = nullptr;
    private:
        std::string m_id = "";
    };
} // namespace ex
