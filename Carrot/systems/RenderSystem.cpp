#include "RenderSystem.h"
#include "Coordinator.hpp"
#include "components/Transform.h"
#include "components/Render.h"

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
    auto const& transformL = gCoordinator.GetComponent<Transform2D>(L);
    auto const& transformR = gCoordinator.GetComponent<Transform2D>(R);

    return transformL.position.y < transformR.position.y;
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

void RenderSystem::update()
{
   // ReorganizeObjects();
}

void RenderSystem::draw()
{
    ReorganizeObjects();
    for (int i = 0; i < draw_order.size(); i++)
    {
        auto const& transform = gCoordinator.GetComponent<Transform2D>(draw_order[i]);
        auto const& rend = gCoordinator.GetComponent<RenderBox>(draw_order[i]);

        Rectangle box = { transform.position.x, transform.position.y, rend.width, rend.height };
        if (rend.image.id != 0)
        {
            Rectangle source = {0, 0, rend.image.width, rend.image.height };
            Rectangle dest = { transform.position.x, transform.position.y, rend.width, rend.height };
            Vector2 origin = { 0, 0 };
            DrawTexturePro(rend.image, source, dest, origin, 0.0f, WHITE);
        }
        //else
        //{
        //  //  DrawRectangleRec(box, ColorAlpha(rend.color, 0.5));
        //}
    }
}