#include "RenderSystem.h"
#include "Coordinator.hpp"
#include "Transform.h"
#include "Render.h"

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
    auto const& transformL = gCoordinator.GetComponent<Transform2D>(L);
    auto const& transformR = gCoordinator.GetComponent<Transform2D>(R);

    return transformL.depth < transformR.depth;
}


void RenderSystem::ReorganizeObjects()
{
    std::sort(draw_order.begin(), draw_order.end(), SortByDepth);
}

void RenderSystem::update()
{
    ReorganizeObjects();
}

void RenderSystem::draw()
{
    for (int i = 0; i < draw_order.size(); i++)
    {
        auto const& transform = gCoordinator.GetComponent<Transform2D>(draw_order[i]);
        auto const& rend = gCoordinator.GetComponent<RenderBox>(draw_order[i]);

        DrawRectangle(transform.position.x, transform.position.y, rend.width, rend.height, rend.color);
    }
}