#include "entity_manager.hpp"


#include <iostream>

namespace ex 
{
    EntityManager entt_man;

    Entity* EntityManager::add_entity(std::string id)
    {
        for (auto& e : entities)
        {
            if(e.id() == id)
            {
                std::cerr << "ERROR: Entity with id: " << id << " already exists." << std::endl;
                return nullptr;
            }
        }
        entities.emplace_back(id);
        return &entities.back();
    }

    Entity* EntityManager::get_entity(std::string id)
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