#pragma once

#include <memory>

#include "transform.hpp"
#include "mesh_comp.hpp"
#include "lights.hpp"


namespace ex
{
    class Entity
    {
    public:
        Entity(std::string_view id);
        std::string_view id() const;
        // components
        std::unique_ptr<Transform> tr = nullptr;
        std::unique_ptr<MeshComponent> mesh = nullptr;
        // std::unique_ptr<BaseShader> shader = nullptr; // TODO: add base material when you done that.
        std::unique_ptr<PointLight> point_light = nullptr;
    private:
        std::string_view m_id = "";
    };
} // namespace ex
