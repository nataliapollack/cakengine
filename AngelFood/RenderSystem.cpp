#include "RenderSystem.h"

#include "Coordinator.hpp"
#include "Core.h"
#include "Player.h"
#include "Game.h"
#include "Progression.h"

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

            Texture2D texture= gAssetMngr.GetAsset(rend.txt);

            // std::cout << stats.type << std::endl;
             Rectangle source = { 0, 0, texture.width, texture.height };
            Vector2 dim = { texture.width * rend.size, texture.height * rend.size };
            Rectangle dest = { transform.pos.x, transform.pos.y, dim.x, dim.y };
            Vector2 origin = { 0, 0 };
            DrawTexturePro(texture, source, dest, origin, 0.0f, WHITE);


        }
    }

    for (auto& entity : entities_list)
    {
        auto& stats = gCoordinator.GetComponent<status>(entity);

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

            if (stats.type == SPIKES)
            {
                DrawRectangleRec(box, ColorAlpha(RED, 0.5));
            }
            if (stats.type == WALL)
            {
                DrawRectangleRec(box, ColorAlpha(GRAY, 0.7));
            }
            if (stats.type == SPAWNER)
            {
                DrawRectangleRec(box, ColorAlpha(YELLOW, 0.5f));
            }
            if (stats.type == SPARK)
            {
                DrawRectangleRec(box, ColorAlpha(SKYBLUE, 0.9f));
            }
            //if (stats.type == EMITTER)
            //{
            //    DrawRectangleRec(box, ColorAlpha(VIOLET, 0.9f));
            //}
            if (stats.type == ITEM)
            {
                DrawRectangleRec(box, ColorAlpha(PINK, 0.9f));
            }
            if (stats.type == DROPOFF)
            {
                DrawRectangleRec(box, ColorAlpha(PURPLE, 0.9f));
            }

            //if (stats.type == ENDPOINT)
            //{
            //    DrawRectangleRec(box, ColorAlpha(GOLD, 0.9f));
            //}
            //if (stats.type == WAYPOINT)
            //{
            //    DrawRectangleRec(box, ColorAlpha(VIOLET, 0.9f));
            //}
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

// anmother day, another rendering system .....
void EnvironmentRenderSystem::init()
{
    for (auto const& entity : entities_list)
    {
        draw_order.push_back(entity);
    }

    ReorganizeObjects();

}

void EnvironmentRenderSystem::draw()
{
    if (draw_order.size() != entities_list.size())
    {
        ReorganizeObjects();
    }

    for (int i = 0; i < draw_order.size(); i++)
    {
        auto const& stats = gCoordinator.GetComponent<status>(draw_order[i]);

        if (stats.active)
        {
            auto const& transform = gCoordinator.GetComponent<transform2D>(draw_order[i]);
            auto const& rend = gCoordinator.GetComponent<render_environment>(draw_order[i]);

            Texture2D texture = gAssetMngr.GetAsset(rend.txt);

            // std::cout << stats.type << std::endl;
            Rectangle source = { 0, 0, texture.width, texture.height };
            if (rend.flip_hor)
            {
                source.width *= -1;
            }
            if (rend.flip_ver)
            {
                source.height *= -1;
            }
            Vector2 dim = { texture.width * rend.size, texture.height * rend.size};
            Rectangle dest = { transform.pos.x, transform.pos.y, dim.x, dim.y };
            Vector2 origin = { 0, 0 };
            DrawTexturePro(texture, source, dest, origin, rend.rotation, WHITE);

            // literally just draw the item the player is holding on 
            if (stats.type == PLAYER)
            {
                auto const& playuh = gCoordinator.GetComponent<player>(draw_order[i]);
                Texture2D halo_texture = gAssetMngr.GetAsset(HALO);
                Vector2 halo_pos = { transform.pos.x + (dim.x / 2.0f) - (halo_texture.width / 2.0f), transform.pos.y - halo_texture.height };

                if (playuh.holding != NONE)
                {
                    Texture2D texture = gAssetMngr.GetAsset(playuh.holding);

                    Rectangle source = { 0, 0, texture.width, texture.height };
                    Vector2 dim2 = { texture.width * 0.5, texture.height * 0.5 };
                    float x = transform.pos.x + (dim.x / 2.0f) - (dim2.x / 2.0f);
                    float y = transform.pos.y - dim2.y + 5;
                    Rectangle dest = { x, y, dim2.x, dim2.y };
                    Vector2 origin = { 0, 0 };
                    DrawTexturePro(texture, source, dest, origin, 0.0f, WHITE);
                    halo_pos.y -= (texture.height / 2.0f);
                }

                if (rend.txt != PLAYER_DEAD || rend.txt != PLAYER_HURT)
                {
                    Rectangle source = { 0, 0, halo_texture.width, halo_texture.height };
                    Rectangle dest = { halo_pos.x, halo_pos.y, halo_texture.width * 0.75f, halo_texture.height * 0.75f };
                    Vector2 origin = { 0, 0 };
                    DrawTexturePro(halo_texture, source, dest, origin, 0.0f, WHITE);
                }
            }
        }
    }

    for (auto& entity : entities_list)
    {
        auto& stats = gCoordinator.GetComponent<status>(entity);

        if (stats.dirty)
        {
            ReorganizeObjects();
            stats.dirty = false;
        }
    }
}

bool SortByActualDepth(Entity const& L, Entity const& R)
{
    auto const& transformL = gCoordinator.GetComponent<render_environment>(L);
    auto const& transformR = gCoordinator.GetComponent<render_environment>(R);

    return transformL.depth < transformR.depth;
}

void EnvironmentRenderSystem::ReorganizeObjects()
{
    if (draw_order.size() != entities_list.size())
    {
        draw_order.clear();
        for (auto const& entity : entities_list)
        {
            draw_order.push_back(entity);
        }
    }

    std::sort(draw_order.begin(), draw_order.end(), SortByActualDepth);
}

void EnvironmentRenderSystem::update()
{
    for (auto const& entity : entities_list)
    {
        auto& rend = gCoordinator.GetComponent<render_environment>(entity);

        if (rend.animate)
        {
            auto& anime = gCoordinator.GetComponent<animate>(entity);
            if (anime.frame_counter >= (60 / anime.speed))
            {
                anime.frame_counter = 0;
                ASSETS temp = rend.txt;

                rend.txt = anime.alt_asset;
                anime.alt_asset = temp;
            }
            anime.frame_counter++;
        }
    }
}