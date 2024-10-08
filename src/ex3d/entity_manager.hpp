#pragma once

#include <vector>

#include "entity.hpp"

namespace ex
{
    class EntityManager
    {
    public:
        std::vector<Entity> entities;

        Entity* add_entity(std::string id);
        Entity* get_entity(std::string id);
    };

    extern EntityManager entt_man;

} // namespace ex
