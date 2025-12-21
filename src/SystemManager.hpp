#pragma once

#include "System.hpp"
#include <memory>
#include <unordered_map>
#include <cassert>

class SystemManager
{
public:
    template<typename T> 
    std::shared_ptr<T> RegisterSystem()
    {
        const char* type_name = typeid(T).name();

        assert(systems_map.find(type_name) == systems_map.end() && "already registered system");

        auto system = std::make_shared<T>();
        systems_map.insert({ type_name, system });
        return system;
    }

    template<typename T>
    void SetSignature(Signature sig)
    {
        const char* type_name = typeid(T).name();

        assert(systems_map.find(type_name) != systems_map.end() && "system used but not registered");

        signatures_map.insert({ type_name, sig });
    }

    void EntityDestroyed(Entity en)
    {
        for (auto const& pair : systems_map)
        {
            auto const& system = pair.second;
            system->entities_list.erase(en);
        }
    }

    void EntitySignatureChanged(Entity en, Signature sig)
    {
        for (auto const& pair : systems_map)
        {
            auto const& type = pair.first;
            auto const& system = pair.second;
            auto const& system_sig = signatures_map[type];

            if ((sig & system_sig) == system_sig)
            {
                system->entities_list.insert(en);
            }
            else
            {
                system->entities_list.erase(en);
            }
        }
    }

private:

    std::unordered_map<const char*, Signature> signatures_map{};
    std::unordered_map<const char*, std::shared_ptr<System>> systems_map{};
};

