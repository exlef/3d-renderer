#pragma once

#include <memory>

#include "transform.hpp"
#include "base_shader.hpp"
#include "mesh_comp.hpp"
#include "lights.hpp"

namespace ex
{
    class Entity
    {
    private:
        std::string_view m_id = "";
    public:
        // components
        std::unique_ptr<Transform> tr = nullptr;
        std::unique_ptr<MeshComponent> mesh = nullptr;
        std::unique_ptr<BaseShader> shader = nullptr;
        std::unique_ptr<PointLight> point_light = nullptr;

        Entity(std::string_view id)
        {
            m_id = id;
        }

        std::string_view id() const { return m_id; }
    };
} // namespace ex
