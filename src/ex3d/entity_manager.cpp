#include "entity_manager.hpp"


#include <iostream>

namespace ex 
{
    EntityManager entt_man;

    // note: we changed form string_view to string because the string view is like char* in C and when it is changing the value that we store as id in entity also changes so it cause problems.
    Entity* EntityManager::add_entity(std::string id)
    {
        std::cout << "current: " <<  id << std::endl;
        for (auto& e : entities)
        {
            std::cout << e.id() << std::endl;
            if(e.id() == id)
            {
                std::cerr << "ERROR: Entity with id: " << id << " already exists." << std::endl;
                return nullptr;
            }
        }
        std::cout << "added: " << id << std::endl; 
        std::cout << "---------------------" << std::endl;
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