#include "RenderingSystem.h"
#include "Coordinator.hpp"

#include "Core.h"
#include "Viewport3D.hpp"
#include "Player.h"

#include <algorithm>
#include <ranges>

extern Coordinator gCoordinator;

static inline bool IsViewport(Entity entity) {
    return gCoordinator.HasComponent<viewport3D>(entity);
}

void RenderSystem::init()
{
    for (auto const& entity : entities_list)
    {
        draw_order.push_back(entity);
    }

    // ReorganizeObjects();
}

void RenderSystem::shutdown() {
    for (Entity en : std::views::filter(draw_order, IsViewport)) {
        auto& fb = gCoordinator.GetComponent<viewport3D>(en).framebuffer;
        UnloadRenderTexture(fb);
    }
}

static bool SortByDepth(uint32_t const& L, uint32_t const& R)
{
    if (gCoordinator.HasComponent<player>(L)) {
        return true;
    }
    else if (gCoordinator.HasComponent<player>(R)) {
        return false;
    }

    auto const& transformL = gCoordinator.GetComponent<transform2D>(L);
    auto const& transformR = gCoordinator.GetComponent<transform2D>(R);

    return transformL.pos.y < transformR.pos.y;
}

void RenderSystem::ReorganizeObjects()
{
    std::sort(draw_order.begin(), draw_order.end(), SortByDepth);
}

void RenderSystem::render_viewports() {
    for (Entity en : std::views::filter(draw_order, IsViewport)) {
        auto& viewport = gCoordinator.GetComponent<viewport3D>(en);
        auto& model = gCoordinator.GetComponent<model_view>(en);
        
        BeginTextureMode(viewport.framebuffer);
        ClearBackground(BLANK);

        BeginMode3D(viewport.view);
        DrawModel(*model.model, model.offset, model.scale, model.color);
        EndMode3D();

        EndTextureMode();
    }
}

void RenderSystem::draw()
{
    for (int i = 0; i < draw_order.size(); i++)
    {
        auto const& transform = gCoordinator.GetComponent<transform2D>(draw_order[i]);

        auto const& rend = gCoordinator.GetComponent<render_box>(draw_order[i]);

        if (gCoordinator.HasComponent<viewport3D>(draw_order[i])) {
            auto const& transform = gCoordinator.GetComponent<transform2D>(draw_order[i]);

            auto const& rend = gCoordinator.GetComponent<render_box>(draw_order[i]);
            auto& viewport = gCoordinator.GetComponent<viewport3D>(draw_order[i]);
            DrawTextureRec(viewport.framebuffer.texture,
                           Rectangle{ 0, 0, rend.size.x, -rend.size.y },
                           Vector2{ transform.pos.x, transform.pos.y },
                           rend.col);
            DrawRectangleLines(transform.pos.x, transform.pos.y, rend.size.x, rend.size.y, RED);
        }
        else if (gCoordinator.HasComponent<render_texture>(draw_order[i]))
        {
            auto const& rend = gCoordinator.GetComponent<render_texture>(draw_order[i]);
            DrawTexture(rend.txt, transform.pos.x, transform.pos.y, WHITE);
        }
        else
        {
            DrawRectangle(int(transform.pos.x), int(transform.pos.y), int(rend.size.x), int(rend.size.y), rend.col);

        }
    }
}