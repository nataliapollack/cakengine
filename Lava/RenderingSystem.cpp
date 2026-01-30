#include "RenderingSystem.h"
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

    // ReorganizeObjects();
}

bool SortByDepth(uint32_t const& L, uint32_t const& R)
{
    auto const& transformL = gCoordinator.GetComponent<transform2D>(L);
    auto const& transformR = gCoordinator.GetComponent<transform2D>(R);

    return transformL.pos.y < transformR.pos.y;
}

void RenderSystem::ReorganizeObjects()
{
    std::sort(draw_order.begin(), draw_order.end(), SortByDepth);
}

void RenderSystem::draw()
{
    for (int i = 0; i < draw_order.size(); i++)
    {
        auto const& transform = gCoordinator.GetComponent<transform2D>(draw_order[i]);
        auto const& rend = gCoordinator.GetComponent<render_box>(draw_order[i]);

        DrawRectangle(transform.pos.x, transform.pos.y, rend.size.x, rend.size.y, rend.col);
    }
}