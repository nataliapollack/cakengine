#include "RenderSystem.h"

#include "Coordinator.hpp"
#include "Core.h"

#include <algorithm>

extern Coordinator gCoordinator;

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
        if (stats.dirty)
        {
            ReorganizeObjects();
        }

        if (stats.active)
        {
            auto const& transform = gCoordinator.GetComponent<transform2D>(draw_order[i]);
            auto const& rend = gCoordinator.GetComponent<render>(draw_order[i]);

            Rectangle box = { transform.pos.x, transform.pos.y, rend.size.x, rend.size.y };
            if (rend.txt.id != 0)
            {
                Rectangle source = { 0, 0, rend.txt.width, rend.txt.height };
                Rectangle dest = { transform.pos.x, transform.pos.y, 244 * 0.5, 269 * 0.5};
                Vector2 origin = { 0, 0 };
                DrawTexturePro(rend.txt, source, dest, origin, 0.0f, WHITE);
            }
           // else
            {
                DrawRectangleRec(box, ColorAlpha(rend.color, 0.5));
            }
        }
    }
}


bool SortByDepth(uint32_t const& L, uint32_t const& R)
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