#include "ECS.h"
#include <algorithm>
#include "../Logger/Logger.h"

int IComponent::nextId = 0;

int Entity::GetId() const
{
    return id;
}

void Entity::Kill() {
    registry->KillEntity(*this);
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

    if(freeIds.empty()) {
        entityId = numEntities++;
        if (entityId >= static_cast<int>(componentSignatures.size()))
        {
            componentSignatures.resize(entityId + 1);
        }
    } else {
        entityId = freeIds.front();
        freeIds.pop_front();
    }


    Entity entity(entityId);
    entity.registry = this;
    entitiesToBeAdded.insert(entity);

    Logger::Log("Entity " + std::to_string(entityId) + " created.");

    return entity;
}

void Registry::KillEntity(Entity entity) {
    entitiesToBeKilled.insert(entity);
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

void Registry::RemoveEntityFromSystems(Entity entity) {
    for(auto system: systems) {
        system.second->RemoveEntity(entity);
    }
}

void Registry::Update() {
    for(auto entity: entitiesToBeAdded) {
        AddEntityToSystems(entity);
    }

    entitiesToBeAdded.clear();

    for(auto entity: entitiesToBeKilled) {
        RemoveEntityFromSystems(entity);
        componentSignatures[entity.GetId()].reset();
        freeIds.push_back(entity.GetId());
        Logger::Log("Entity " + std::to_string(entity.GetId()) + " was removed.");
    }

    entitiesToBeKilled.clear();
}
