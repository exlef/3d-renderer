#include "entity_manager.hpp"

namespace ex 
{
    Entity* EntityManager::add_entity(std::string_view id)
    {
        entities.emplace_back(id);
        return &entities.back();
    }

    Entity* EntityManager::get_entity(std::string_view id)
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
}