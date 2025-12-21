#pragma once
#include "Types.hpp"
#include <cassert>

class EntityManager
{
public:
    EntityManager()
    {
        for (int i = 0; i < MAX_ENTITIES; i++)
        {
            entity_list[i] = i;
        }
    }

    Entity CreateEntity()
    {
        assert(alive_entity_count < MAX_ENTITIES && "Max entities reached.");

        Entity id = entity_list[alive_entity_count];
        ++alive_entity_count;

        return id;
    }
    void DestroyEntity(Entity entity)
    {
        assert(entity < MAX_ENTITIES && "Entity out of range");

        signature_list[entity].reset();
        --alive_entity_count;
    }

    void SetSignature(Entity entity, Signature sig)
    {
        assert(entity < MAX_ENTITIES && "Entity out of range.");

        signature_list[entity] = sig;
    }
    Signature GetSignature(Entity entity)
    {
        assert(entity < MAX_ENTITIES && "Entity out of range.");

        return signature_list[entity];
    }

private:
    uint32_t alive_entity_count{};
    Entity entity_list[MAX_ENTITIES];
    Signature signature_list[MAX_ENTITIES];
};