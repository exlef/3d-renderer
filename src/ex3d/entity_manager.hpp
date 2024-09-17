#pragma once

#include <vector>

#include "entity.hpp"

namespace ex
{
    class EntityManager
    {
    private:
        std::vector<Entity> m_entities;
    public:
        Entity* add_entity(std::string_view id)
        {
            m_entities.emplace_back(id);
            return &m_entities.back();
        }

        Entity* get_entity(std::string_view id)
        {
            for(auto& e: m_entities)
            {
                if(e.id() == id)
                {
                    return &e;
                }
            }

            std::cout << "ERROR: these is no entitiy with id: " << id << std::endl;
            return nullptr;
        }
    };
} // namespace ex
