#pragma once

#include "Types.hpp"
#include <unordered_map>
#include <cassert>
#include <array>

class IComponentArray
{
public: 
    virtual ~IComponentArray() = default;
    virtual void EntityDestroyed(Entity en) = 0;
};

template<typename T>
class ComponentArray : public IComponentArray
{
public:
    void InsertData(Entity en, T component)
    {
        assert(entity_to_index_map.find(en) == entity_to_index_map.end() && "component added to same entity!");

        size_t new_ind = size;
        entity_to_index_map[en] = new_ind;
        index_to_entity_map[new_ind] = en;
        component_array[new_ind] = component;
        ++size;
    }

    void RemoveData(Entity en)
    {
        assert(entity_to_index_map.find(en) != entity_to_index_map.end() && "entity does not have component");

        size_t index_of_removed_en = entity_to_index_map[en];
        size_t index_of_last_el = size - 1;

        component_array[index_of_removed_en] = component_array[index_of_last_el];

        Entity entity_last_el = index_to_entity_map[index_of_last_el];
        entity_to_index_map[entity_last_el] = index_of_removed_en;
        index_to_entity_map[index_of_removed_en] = entity_last_el;

        entity_to_index_map.erase(en);
        index_to_entity_map.erase(index_of_last_el);

        --size;
    }

    T& GetData(Entity en)
    {
        assert(entity_to_index_map.find(en) != entity_to_index_map.end() && "retrieving non-existant component");

        return component_array[entity_to_index_map[en]];
    }

    void EntityDestroyed(Entity en) override
    {
        if (entity_to_index_map.find(en) != entity_to_index_map.end())
        {
            RemoveData(en);
        }
    }

private:

    std::array<T, MAX_ENTITIES> component_array;
    std::unordered_map<Entity, size_t> entity_to_index_map;
    std::unordered_map<size_t, Entity> index_to_entity_map;

    size_t size;
};