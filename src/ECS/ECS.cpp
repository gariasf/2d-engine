#include "ECS.h"
#include <algorithm>
#include "../Logger/Logger.h"

int IComponent::nextId = 0;

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
    entities.erase(std::remove_if(entities.begin(), entities.end(), [&entity](Entity other)
                                  { return entity == other; }),
                   entities.end());
}

std::vector<Entity> System::GetEntities() const
{
    return entities;
}

const Signature &System::GetSignature() const
{
    return componentSignature;
}

Entity Registry::CreateEntity() {
    int entityId;

    entityId = numEntities++;

    Entity entity(entityId);
    entitiesToBeAdded.insert(entity);

    Logger::Log("Entity created with id = " + std::to_string(entityId));
    return entity;
}

void Registry::AddEntityToSystems(Entity entity) {
    const auto entityId = entity.GetId();

    const auto& entityComponentSignature = componentSignatures[entityId];

    for(auto& system: systems) {
        const auto& systemComponentSignature = system.second->GetSignature();
        bool isInterested = (entityComponentSignature & systemComponentSignature) == systemComponentSignature;

        if(isInterested) {
            system.second->AddEntity(entity);
        }

    }
}

void Registry::Update() {

}
