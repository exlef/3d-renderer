#pragma once

#include <vector>

#include "entity.hpp"

namespace ex
{
    class EntityManager
    {
    private:
        
    public:
        std::vector<Entity> entities;

        Entity* add_entity(std::string_view id)
        {
            entities.emplace_back(id);
            return &entities.back();
        }

        Entity* get_entity(std::string_view id)
        {
            for (auto& e : entities)
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
