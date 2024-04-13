#include "Entity.hpp"
    
    Entity::Entity(Type type)
    {
        entity_type = type;
    }
    
    Entity::Type Entity::getEntityType()
    {
        return entity_type;
    }
    void Entity::setEntityType(Type type)
    {
        entity_type = type;
    }

