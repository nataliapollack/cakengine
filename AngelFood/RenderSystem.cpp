#include "RenderSystem.h"

#include "Coordinator.hpp"
#include "Core.h"
#include "Player.h"
#include "Game.h"

#include "AssetManager.h"

#include <algorithm>

#include <iostream>

extern Coordinator gCoordinator;
extern AssetManager gAssetMngr;

void RenderSystem::init()
{
    for (auto const& entity : entities_list)
    {
        draw_order.push_back(entity);
    }

    ReorganizeObjects();
}


void RenderSystem::draw()
{
    for (int i = 0; i < draw_order.size(); i++)
    {
        auto const& stats = gCoordinator.GetComponent<status>(draw_order[i]);

        if (stats.active)
        {
            auto const& transform = gCoordinator.GetComponent<transform2D>(draw_order[i]);
            auto const& rend = gCoordinator.GetComponent<render>(draw_order[i]);

            Texture2D texture= gAssetMngr.GetAsset(draw_order[i]);

            // std::cout << stats.type << std::endl;
            Rectangle source = { 0, 0, texture.width, texture.height };
            Vector2 dim = { texture.width * rend.size, texture.height * rend.size };
            Rectangle dest = { transform.pos.x, transform.pos.y, dim.x, dim.y };
            Vector2 origin = { 0, 0 };
            DrawTexturePro(texture, source, dest, origin, 0.0f, WHITE);

            // literally just draw the item the player is holding on 
            if (stats.type == PLAYER)
            {
                auto const& playuh = gCoordinator.GetComponent<player>(draw_order[i]);

                if (playuh.holding != NONE)
                {
                    Texture2D texture = gAssetMngr.GetAsset(playuh.holding);

                    Rectangle source = { 0, 0, texture.width, texture.height };
                    Vector2 dim2 = { texture.width * 0.5, texture.height * 0.5 };
                    float x = transform.pos.x + (dim.x / 2.0f) - (dim2.x / 2.0f);
                    float y = transform.pos.y - dim2.y / 2.0f;
                    Rectangle dest = { x, y, dim2.x, dim2.y};
                    Vector2 origin = { 0, 0 };
                    DrawTexturePro(texture, source, dest, origin, 0.0f, WHITE);
                }
            }
        }
    }

    for (int i = 0; i < draw_order.size(); i++)
    {
        auto& stats = gCoordinator.GetComponent<status>(draw_order[i]);

        if (stats.dirty)
        {
            ReorganizeObjects();
            stats.dirty = false;
        }
    }


}


bool SortByDepth(Entity const& L, Entity const& R)
{
    auto const& transformL = gCoordinator.GetComponent<transform2D>(L);
    auto const& transformR = gCoordinator.GetComponent<transform2D>(R);

    return transformL.pos.y < transformR.pos.y;
}

void RenderSystem::ReorganizeObjects()
{
    if (draw_order.size() != entities_list.size())
    {
        draw_order.clear();
        for (auto const& entity : entities_list)
        {
            draw_order.push_back(entity);
        }
    }

    std::sort(draw_order.begin(), draw_order.end(), SortByDepth);
}

void BoxRenderSystem::init()
{
    for (auto const& entity : entities_list)
    {
        draw_order.push_back(entity);
    }

    ReorganizeObjects();
}

void BoxRenderSystem::draw()
{
    for (int i = 0; i < draw_order.size(); i++)
    {
        auto const& stats = gCoordinator.GetComponent<status>(draw_order[i]);
        if (stats.dirty)
        {
            ReorganizeObjects();
        }

        if (stats.active)
        {
            auto const& transform = gCoordinator.GetComponent<transform2D>(draw_order[i]);
            auto const& rend = gCoordinator.GetComponent<box_render>(draw_order[i]);

            Rectangle box = { transform.pos.x, transform.pos.y, rend.dimensions.x, rend.dimensions.y };
            DrawRectangleRec(box, ColorAlpha(rend.color, 1.0));
            
        }
    }
}

void BoxRenderSystem::ReorganizeObjects()
{
    if (draw_order.size() != entities_list.size())
    {
        draw_order.clear();
        for (auto const& entity : entities_list)
        {
            draw_order.push_back(entity);
        }
    }

    std::sort(draw_order.begin(), draw_order.end(), SortByDepth);
}