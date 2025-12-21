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

bool SortByDepth(uint32_t const& L, uint32_t const& R)
{
    auto const& transformL = gCoordinator.GetComponent<transform2>(L);
    auto const& transformR = gCoordinator.GetComponent<transform2>(R);

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

void RenderSystem::draw()
{
    ReorganizeObjects();
    for (int i = 0; i < draw_order.size(); i++)
    {
        auto const& transf = gCoordinator.GetComponent<transform2>(draw_order[i]);
        auto const& rend = gCoordinator.GetComponent<render_box>(draw_order[i]);
        auto const& stats = gCoordinator.GetComponent<status>(draw_order[i]);

        if (stats.active == true)
        {
            Rectangle box = { transf.pos.x, transf.pos.y, rend.size.x, rend.size.y };
            if (rend.txt.id != 0)
            {
                Rectangle source = { 0, 0, rend.txt.width, rend.txt.height };
                Rectangle dest = { transf.pos.x, transf.pos.y,box.width, box.height};
                Vector2 origin = { 0, 0 };
                DrawTexturePro(rend.txt, source, dest, origin, 0.0f, WHITE);
            }
            else
            {
                DrawRectangleRec(box, ColorAlpha(rend.color, 0.5));
            }
        }
    }
}