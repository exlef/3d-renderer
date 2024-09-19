#pragma once

#include <vector>

#include "entity.hpp"

namespace ex
{
    class EntityManager
    {
    public:
        std::vector<Entity> entities;

        Entity* add_entity(std::string_view id);
        Entity* get_entity(std::string_view id);
    };
} // namespace ex
