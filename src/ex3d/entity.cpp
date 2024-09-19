#include "entity.hpp"

namespace ex 
{
    Entity::Entity(std::string_view id)
    {
        m_id = id;
    }

    std::string_view Entity::id() const 
    {
         return m_id; 
    }
}