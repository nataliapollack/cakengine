#pragma once
#include <unordered_map>
#include <memory>
#include "Types.hpp"
#include "Components.hpp"

class ComponentManager
{
public:
    template<typename T>
    void RegisterComponent()
    {
        const char* type_name = typeid(T).name();

        assert(component_types.find(type_name) == component_types.end() && "registering component more than once!");

        component_types.insert({ type_name, next_comp_type });

        component_arrays.insert({ type_name, std::make_shared<ComponentArray<T>>() });

        ++next_comp_type;
    }

    template<typename T>
    ComponentType GetComponentType()
    {
        const char* type_name = typeid(T).name();

        assert(component_types.find(type_name) != component_types.end() && "attempting to use a component not registered!");

        return component_types[type_name];
    }

    template<typename T>
    void AddComponent(Entity en, T comp)
    {
        GetComponentArray<T>()->InsertData(en, comp);
    }

    template<typename T>
    void RemoveComponent(Entity en)
    {
        GetComponentArray<T>()->RemoveData(en);
    }

    template<typename T>
    T& GetComponent(Entity en)
    {
        return GetComponentArray<T>()->GetData(en);
    }

    void EntityDestroyed(Entity en)
    {
        for (auto const& pair : component_arrays)
        {
            auto const& comp = pair.second;
            comp->EntityDestroyed(en);
        }
    }

private:

    template<typename T> 
    std::shared_ptr<ComponentArray<T>> GetComponentArray()
    {
        const char* type_name = typeid(T).name();

        assert(component_types.find(type_name) != component_types.end() && "attempting to get a component not registered!");

        return std::static_pointer_cast<ComponentArray<T>>(component_arrays[type_name]);
    }

    std::unordered_map<const char*, ComponentType> component_types{};
    std::unordered_map<const char*, std::shared_ptr<IComponentArray>> component_arrays{};
    ComponentType next_comp_type{};
};