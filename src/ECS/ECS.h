#pragma once

#include <bitset>
#include <vector>

const unsigned int MAX_COMPONENTS = 32;
typedef std::bitset<MAX_COMPONENTS> Signature;

struct BaseComponent {
    protected:
        static int nextId;
};

template <typename TComponent>
class Component : public BaseComponent
{
    static int GetId() {
        static auto id = nextId++;
        return id;

    }
};

class Entity {
    private:
        int id;
    public:
        Entity(int id): id(id) {}
        int GetId() const;
};

class System {
    private:
        Signature componentSignature;
        std::vector<Entity> entities;
    public:
        System() = default;
        ~System() = default;

        void AddEntity(Entity entity);
        void RemoveEntity(Entity entity);
        std::vector<Entity> GetEntities() const;
        const Signature& GetSignature() const;

        template <typename TComponent> void RequireComponent();
};

class Registry {};

template <typename TComponent>
void System::RequireComponent()
{
    const auto componentId = Component<TComponent>::GetId();
    componentSignature.set(componentId);
}