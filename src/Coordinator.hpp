#include "SystemManager.hpp"
#include "ComponentManager.hpp"
#include "EntityManager.hpp"
#include "EventManager.hpp"
#include "Types.hpp"

class Coordinator
{
public: 
    void init()
    {
        entity_manager = std::make_unique<EntityManager>();
        event_manager = std::make_unique<EventManager>();
        component_manager = std::make_unique<ComponentManager>();
        system_manager = std::make_unique<SystemManager>();
    }

    Entity CreateEntity()
    {
        return entity_manager->CreateEntity();
    }
    void DestroyEntity(Entity en)
    {
        entity_manager->DestroyEntity(en);

        component_manager->EntityDestroyed(en);

        system_manager->EntityDestroyed(en);
    }

    template<typename T>
    void RegisterComponent()
    {
        component_manager->RegisterComponent<T>();
    }
    template<typename T>
    void AddComponent(Entity en, T comp)
    {
        component_manager->AddComponent<T>(en, comp);

        auto signature = entity_manager->GetSignature(en);
        signature.set((component_manager->GetComponentType<T>()), true);
        entity_manager->SetSignature(en, signature);

        system_manager->EntitySignatureChanged(en, signature);
    }
    template<typename T>
    void RemoveComponent(Entity en)
    {
        component_manager->RemoveComponent<T>(en);

        auto signature = entity_manager->GetSignature(en);
        signature.set(component_manager->GetComponentType<T>(), false);
        entity_manager->SetSignature(en, signature);

        system_manager->EntitySignatureChanged(en, signature);
    }
    template<typename T>
    T& GetComponent(Entity en)
    {
        return component_manager->GetComponent<T>(en);
    }
    template<typename T>
    ComponentType GetComponentType()
    {
        return component_manager->GetComponentType<T>();
    }

    template<typename T>
    std::shared_ptr<T> RegisterSystem()
    {
        return system_manager->RegisterSystem<T>();
    }
    template<typename T>
    void SetSystemSignature(Signature sig)
    {
        system_manager->SetSignature<T>(sig);
    }
    // Event methods
    void AddEventListener(EventId eventId, std::function<void(Event&)> const& listener)
    {
        event_manager->AddListener(eventId, listener);
    }

    void SendEvent(Event& event)
    {
        event_manager->SendEvent(event);
    }

    void SendEvent(EventId eventId)
    {
        event_manager->SendEvent(eventId);
    }

private:
    std::unique_ptr<EntityManager> entity_manager;
    std::unique_ptr<EventManager> event_manager;
    std::unique_ptr<ComponentManager> component_manager;
    std::unique_ptr<SystemManager> system_manager;
};