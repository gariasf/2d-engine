#include "ECS.h"

int Entity::GetId() const
{
    return id;
}

void System::AddEntity(Entity entity)
{
    entities.push_back(entity);
}

void System::RemoveEntity(Entity entity)
{
    entities.erase(std::remove_if(entities.begin(), entites.end(), [&entity](Entity other)
                                  { return entity == other; }),
                   entites.end());
}

std::vector<Entity> System::GetEntities() const
{
    return entities;
}

const Signature &System::GetSignature() const
{
    return componentSignature;
}