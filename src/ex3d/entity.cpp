#include "entity.hpp"

namespace ex 
{
    Entity::Entity(std::string id)
    {
        m_id = id;
    }

    std::string Entity::id() const 
    {
         return m_id; 
    }
}