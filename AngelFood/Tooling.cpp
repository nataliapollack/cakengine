#include "Tooling.h"
#include "Game.h"
#include "Coordinator.hpp"
#include "AssetManager.h"

// components
#include "Core.h"
#include "Player.h"
#include "Progression.h"
#include "ParticleEmitter.h"

#include <iostream>
#include <string>
#include <fstream>
#include "raymath.h"


#define MOUSE_SCALE_MARK_SIZE   12

extern Coordinator gCoordinator;
extern Camera2D gCamera;
extern AssetManager gAssetMngr;


void Tooling::serialize()
{
    std::ofstream data;
    data.open("level/entities.txt");

    if (data.is_open())
    {
        for (auto& entity : entities_list)
        {
            data << "EN" << "\n";
            if (gCoordinator.HasComponent<transform2D>(entity))
            {
                auto& comp = gCoordinator.GetComponent<transform2D>(entity);
                data << TRANSFORM << "\n";
                data << comp.pos.x << "\n";
                data << comp.pos.y << "\n";
            }
            if (gCoordinator.HasComponent<render>(entity))
            {
                auto& comp = gCoordinator.GetComponent<render>(entity);
                data << RENDER << "\n";
                data << comp.size << "\n";
                data << comp.txt << "\n";
            }
            if (gCoordinator.HasComponent<box_render>(entity))
            {
                auto& comp = gCoordinator.GetComponent<box_render>(entity);
                data << BOX_RENDER<< "\n";
                data << comp.dimensions.x << "\n";
                data << comp.dimensions.y << "\n";
                data << comp.platform << "\n";
            }
            if (gCoordinator.HasComponent<collidble>(entity))
            {
                auto& comp = gCoordinator.GetComponent<collidble>(entity);
                data << COLLIDBLE << "\n";
                data << comp.box.x << "\n";
                data << comp.box.y << "\n";
                data << comp.box.width << "\n";
                data << comp.box.height << "\n";
            }
            if (gCoordinator.HasComponent<physics>(entity))
            {
                auto& comp = gCoordinator.GetComponent<physics>(entity);
                data << PHYSICS << "\n";
                data << comp.vel.x << "\n";
                data << comp.vel.y << "\n";
                data << comp.f.x << "\n";
                data << comp.f.y << "\n";
            }
            if (gCoordinator.HasComponent<status>(entity))
            {
                auto& comp = gCoordinator.GetComponent<status>(entity);
                data << STATUS << "\n";
                data << comp.active << "\n";
                data << comp.dirty << "\n";
                data << comp.type << "\n";
            }
            if (gCoordinator.HasComponent<player>(entity))
            {
                auto& comp = gCoordinator.GetComponent<player>(entity);
                data << PLAYERc << "\n";
                data << comp.on_ground << "\n";
                data << comp.holding << "\n";
            }
            if (gCoordinator.HasComponent<collectable>(entity))
            {
                auto& comp = gCoordinator.GetComponent<collectable>(entity);
                data << COLLECTABLE << "\n";
                data << comp.picked_up << "\n";
            }
            if (gCoordinator.HasComponent<collecting>(entity))
            {
                auto& comp = gCoordinator.GetComponent<collecting>(entity);
                data << COLLECTING << "\n";
                data << comp.item << "\n";
            }
            if (gCoordinator.HasComponent<render_environment>(entity))
            {
                auto& comp = gCoordinator.GetComponent<render_environment>(entity);
                data << ENVIRONMENT_RENDER << "\n";
                data << comp.animate << "\n";
                data << comp.flip_ver << "\n";
                data << comp.flip_hor << "\n";
                data << comp.txt << "\n";
                data << comp.depth << "\n";
                data << comp.size << "\n";
                data << comp.rotation << "\n";
            }
            if (gCoordinator.HasComponent<animate>(entity))
            {
                auto& comp = gCoordinator.GetComponent<animate>(entity);
                data << ANIMATE << "\n";
                data << comp.speed << "\n";
                data << comp.frame_counter << "\n";
                data << comp.alt_asset << "\n";
            }
            if (gCoordinator.HasComponent<particle_emitter>(entity))
            {
                auto& comp = gCoordinator.GetComponent<particle_emitter>(entity);
                data << EMITTER << "\n";
                data << comp.capacity << "\n";
                data << comp.alive_count << "\n";

                data << comp.offset.first.x << "\n";
                data << comp.offset.first.y << "\n";
                data << comp.offset.second.x << "\n";
                data << comp.offset.second.y << "\n";

                data << static_cast<int>(comp.color.r) << "\n";
                data << static_cast<int>(comp.color.g) << "\n";
                data << static_cast<int>(comp.color.b) << "\n";
                data << static_cast<int>(comp.color.a) << "\n";

                data << comp.initial_dir.x << "\n";
                data << comp.initial_dir.y << "\n";

                data << comp.max_angle_variation << "\n";

                data << comp.initial_speed.first << "\n";
                data << comp.initial_speed.second << "\n";

                data << comp.initial_lifetime.first << "\n";
                data << comp.initial_lifetime.second << "\n";

                data << comp.initial_size.first << "\n";
                data << comp.initial_size.second << "\n";

                data << comp.num_per_emit << "\n";
                data << comp.emitting << "\n";
                data << comp.one_shot << "\n";
                data << comp.texture_id << "\n";

                data << comp.time_between_emit.time() << "\n";
                data << comp.type << "\n";
            }
            if (gCoordinator.HasComponent<waypoint>(entity))
            {
                auto& comp = gCoordinator.GetComponent<waypoint>(entity);
                data << WAYPOINT << "\n";
                data << comp.index << "\n";
                data << comp.should_stop << "\n";
            }
        }
    }

    data.close();
}

void Tooling::deserialize()
{
    std::string line;
    std::ifstream load_data("level/entities.txt");

    // have to get it started
    Entity en = gCoordinator.CreateEntity();
    std::getline(load_data, line);

    if (load_data.is_open())
    {
        while (std::getline(load_data, line))
        {            
            
            if (TRANSFORM == atoi(line.c_str()))
            {
                std::getline(load_data, line);
                float x = atof(line.c_str());
                std::getline(load_data, line);
                float y = atof(line.c_str());

                gCoordinator.AddComponent(en,
                    transform2D{ Vector2 {x, y} });

                std::getline(load_data, line);

            }
            if (RENDER == atoi(line.c_str()))
            {
                std::getline(load_data, line);
                float x = atof(line.c_str());
                std::getline(load_data, line);
                int y = atoi(line.c_str());

                gCoordinator.AddComponent(en,
                    render{ x, (ASSETS)y });

                std::getline(load_data, line);
            }
            if (BOX_RENDER == atoi(line.c_str()))
            {
                std::getline(load_data, line);
                float x = atof(line.c_str());
                std::getline(load_data, line);
                float y = atof(line.c_str());

                std::getline(load_data, line);
                bool z = atoi(line.c_str());

                gCoordinator.AddComponent(en,
                    box_render{ x, y, z });

                std::getline(load_data, line);
            }
            if (COLLIDBLE == atoi(line.c_str()))
            {
                std::getline(load_data, line);
                float x = atof(line.c_str());
                std::getline(load_data, line);
                float y = atof(line.c_str());

                std::getline(load_data, line);
                float w = atof(line.c_str());
                std::getline(load_data, line);
                float h = atof(line.c_str());

                gCoordinator.AddComponent(en,
                    collidble{ Rectangle{x, y, w, h } });

                std::getline(load_data, line);
            }
            if (PHYSICS == atoi(line.c_str()))
            {
                std::getline(load_data, line);
                float x = atof(line.c_str());
                std::getline(load_data, line);
                float y = atof(line.c_str());

                std::getline(load_data, line);
                float w = atof(line.c_str());
                std::getline(load_data, line);
                float h = atof(line.c_str());

                gCoordinator.AddComponent(en,
                    physics{ Vector2{x, y}, Vector2{ w, h }});

                std::getline(load_data, line);
            }
            if (STATUS == atoi(line.c_str()))
            {
                std::getline(load_data, line);
                bool x = atoi(line.c_str());
                std::getline(load_data, line);
                bool y = atoi(line.c_str());

                std::getline(load_data, line);
                int z = atoi(line.c_str());

                gCoordinator.AddComponent(en,
                    status{ x, y, OBJECT_TYPE(z)});

                std::getline(load_data, line);
            }
            if (PLAYERc == atoi(line.c_str()))
            {
                std::getline(load_data, line);
                bool x = atoi(line.c_str());
                std::getline(load_data, line);
                int y = atoi(line.c_str());

                gCoordinator.AddComponent(en,
                    player{ x, (HOLDABLE_ITEMS)y });

                std::getline(load_data, line);
            }
            if (COLLECTABLE == atoi(line.c_str()))
            {
                std::getline(load_data, line);
                bool x = atoi(line.c_str());

                gCoordinator.AddComponent(en,
                    collectable{ x });

                std::getline(load_data, line);
            }
            if (COLLECTING == atoi(line.c_str()))
            {
                std::getline(load_data, line);
                bool x = atoi(line.c_str());

                gCoordinator.AddComponent(en,
                    collecting{ (HOLDABLE_ITEMS)x });

                std::getline(load_data, line);
            }
            if (ENVIRONMENT_RENDER == atoi(line.c_str()))
            {
                std::getline(load_data, line);
                bool x = atoi(line.c_str());

                std::getline(load_data, line);
                bool y = atoi(line.c_str());

                std::getline(load_data, line);
                bool z = atoi(line.c_str());

                std::getline(load_data, line);
                int txt = atoi(line.c_str());

                std::getline(load_data, line);
                int a = atoi(line.c_str());

                std::getline(load_data, line);
                float b = atof(line.c_str());

                std::getline(load_data, line);
                float c = atof(line.c_str());

                gCoordinator.AddComponent(en,
                    render_environment{x, y, z, (ASSETS)txt, a, b, c});

                std::getline(load_data, line);
            }
            if (ANIMATE == atoi(line.c_str()))
            {
                std::getline(load_data, line);
                float a = atof(line.c_str());

                std::getline(load_data, line);
                int b = atoi(line.c_str());

                std::getline(load_data, line);
                int c = atoi(line.c_str());

                gCoordinator.AddComponent(en,
                    animate{ a, b, (ASSETS)c});

                std::getline(load_data, line);
            }
            if (EMITTER == atoi(line.c_str()))
            {
                std::getline(load_data, line);
                size_t n = atoi(line.c_str());

                std::getline(load_data, line);
                size_t ac = atoi(line.c_str());

                // offset
                std::getline(load_data, line);
                float ofx = atof(line.c_str());
                std::getline(load_data, line);
                float ofy = atof(line.c_str());
                std::getline(load_data, line);
                float osx = atof(line.c_str());
                std::getline(load_data, line);
                float osy = atof(line.c_str());

                // color
                std::getline(load_data, line);
                unsigned char cr = atoi(line.c_str());
                std::getline(load_data, line);
                unsigned char cg = atoi(line.c_str());
                std::getline(load_data, line);
                unsigned char cb = atoi(line.c_str());
                std::getline(load_data, line);
                unsigned char ca = atoi(line.c_str());

                // inital dir
                std::getline(load_data, line);
                float idx = atof(line.c_str());
                std::getline(load_data, line);
                float idy = atof(line.c_str());

                // angle variation
                std::getline(load_data, line);
                float av = atof(line.c_str());

                // initial speed
                std::getline(load_data, line);
                float isfx = atof(line.c_str());
                std::getline(load_data, line);
                float isfy = atof(line.c_str());

                // initial lifetime
                std::getline(load_data, line);
                float ilfx = atof(line.c_str());
                std::getline(load_data, line);
                float ilfy = atof(line.c_str());

                // initial size
                std::getline(load_data, line);
                float izfx = atof(line.c_str());
                std::getline(load_data, line);
                float izfy = atof(line.c_str());

                // num per emit
                std::getline(load_data, line);
                size_t ne = atoi(line.c_str());

                // emit
                std::getline(load_data, line);
                bool e = atoi(line.c_str());

                // one shot
                std::getline(load_data, line);
                bool os = atoi(line.c_str());

                // texture id
                ASSETS tid = static_cast<ASSETS>(atoi(line.c_str()));

                // timer
                std::getline(load_data, line);
                float tr = atof(line.c_str());

                // type
                std::getline(load_data, line);
                int t = atoi(line.c_str());

                gCoordinator.AddComponent(en,
                    particle_emitter
                    {
                        n,
                        ac,
                        { Vector2{ ofx, ofy }, Vector2{ osx, osy } },
                        { cr, cg, cb, ca },
                        { idx, idy },
                        av,
                        { isfx, isfy },
                        { ilfx, ilfy },
                        { izfx, izfy },
                        ne,
                        e,
                        os,
                        tid,
                        tr,
                        static_cast<emitter_type>(t),
                        {}
                    });

                std::getline(load_data, line);
            }
            if (WAYPOINT == atoi(line.c_str()))
            {
                std::getline(load_data, line);
                size_t idx = atoi(line.c_str());
                std::getline(load_data, line);
                bool ss = atoi(line.c_str());

                gCoordinator.AddComponent(en,
                    waypoint{ idx, ss });

                std::getline(load_data, line);
            }
            
            if ("EN" == line)
            {
                en = gCoordinator.CreateEntity();
            }
            
        }
    }
}

bool Tooling::GetToolStatus()
{
    return tooling_on;
}



void Tooling::update()
{
    if (IsKeyPressed(KEY_ONE))
    {
        tooling_on = !tooling_on;
    }

    if (tooling_on)
    {
        if (!mouseMoveRect && !mouseScaleMode)
        {
            for (auto& entity : entities_list)
            {
                auto const& stats = gCoordinator.GetComponent<status>(current_en);

                Vector2 mouse_pos = GetScreenToWorld2D(GetMousePosition(), gCamera);
                auto& transform = gCoordinator.GetComponent<transform2D>(entity);
                Rectangle rec = Rectangle{ transform.pos.x, transform.pos.y, 100, 100 };

                if (gCoordinator.HasComponent<box_render>(entity) && !only_walls)
                {
                    auto& box = gCoordinator.GetComponent<box_render>(entity);
                    rec.width = box.dimensions.x;
                    rec.height = box.dimensions.y;
                }
                if (gCoordinator.HasComponent<render_environment>(entity) && !only_env)
                {
                   // continue;
                    auto& rend = gCoordinator.GetComponent<render_environment>(entity);
                    Texture2D texture = gAssetMngr.GetAsset(rend.txt);

                    rec.width = texture.width;
                    rec.height = texture.height ;
                }

                Rectangle rec2 = { rec.x + rec.width - MOUSE_SCALE_MARK_SIZE, rec.y + rec.height - MOUSE_SCALE_MARK_SIZE, MOUSE_SCALE_MARK_SIZE, MOUSE_SCALE_MARK_SIZE };


                if (CheckCollisionPointRec(mouse_pos, rec2) && stats.active)
                {
                    current_rec = rec;
                    mouseScaleReady = true;
                    current_en = entity;
                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                    {
                        mouseScaleMode = true;
                        continue;
                    }
                }
                else mouseScaleReady = false;

                if (CheckCollisionPointRec(mouse_pos, rec))
                {
                    current_rec = rec;
                    current_en = entity;
                    if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT) && stats.active)
                    {
                        mouseMoveRect = true;

                        mOffset = Vector2Subtract(Vector2{ transform.pos.x,  transform.pos.y }, mouse_pos);
                        continue;
                    }
                }


            }
        }

        if (mouseScaleMode)
        {
            Vector2 mouse_pos = GetScreenToWorld2D(GetMousePosition(), gCamera);
            auto& transform = gCoordinator.GetComponent<transform2D>(current_en);

            Rectangle rec = Rectangle{ transform.pos.x, transform.pos.y, 100, 100 };

            if (gCoordinator.HasComponent<box_render>(current_en))
            {
                auto& box = gCoordinator.GetComponent<box_render>(current_en);
                rec.width = box.dimensions.x;
                rec.height = box.dimensions.y;
            }

            if (gCoordinator.HasComponent<render_environment>(current_en))
            {
                auto& rend = gCoordinator.GetComponent<render_environment>(current_en);
                Texture2D txt = gAssetMngr.GetAsset(rend.txt);

                rec.width = txt.width * rend.size;
                rec.height = txt.height * rend.size;
            }

            mouseScaleReady = true;

            rec.width = (int)((mouse_pos.x - rec.x) / 50.0f) * 50.0f;
            rec.height = (int)((mouse_pos.y - rec.y) / 50.0f) * 50.0f;

            if (rec.width < 25) rec.width = 25.0f;
            if (rec.height < 25) rec.height = 25.0f;

            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) mouseScaleMode = false;

            current_rec = rec;

            if (gCoordinator.HasComponent<box_render>(current_en))
            {
                auto& box = gCoordinator.GetComponent<box_render>(current_en);
                box.dimensions.x = rec.width;
                box.dimensions.y = rec.height;

                auto& collision = gCoordinator.GetComponent<collidble>(current_en);
                collision.box = rec;
            }

            if (gCoordinator.HasComponent<render_environment>(current_en))
            {
                auto& rend = gCoordinator.GetComponent<render_environment>(current_en);
                Texture2D txt = gAssetMngr.GetAsset(rend.txt);

                rend.size = rec.width / txt.width;
            }
        }

        if (mouseMoveRect)
        {
            Vector2 mouse_pos = GetScreenToWorld2D(GetMousePosition(), gCamera);
            auto& transform = gCoordinator.GetComponent<transform2D>(current_en);

            Rectangle rec = Rectangle{ transform.pos.x, transform.pos.y, 100, 100 };
            if (gCoordinator.HasComponent<collidble>(current_en))
            {
                auto& collision = gCoordinator.GetComponent<collidble>(current_en);
                rec = collision.box;
            }
            if (gCoordinator.HasComponent<render_environment>(current_en))
            {
                auto& rend = gCoordinator.GetComponent<render_environment>(current_en);
                Texture2D texture = gAssetMngr.GetAsset(rend.txt);

                rec.width = texture.width;
                rec.height = texture.height;
            }

            transform.pos = Vector2Add(mouse_pos, mOffset);
            transform.pos.x = (int)(transform.pos.x / 25.0f) * 25.0f;
            transform.pos.y = (int)(transform.pos.y / 25.0f) * 25.0f;


            if (gCoordinator.HasComponent<collidble>(current_en))
            {
                auto& collision = gCoordinator.GetComponent<collidble>(current_en);
                collision.box.x = transform.pos.x;
                collision.box.y = transform.pos.y;

                auto& stats = gCoordinator.GetComponent<status>(current_en);
                if (stats.type == WAYPOINT)
                {
                    collision.box.x = transform.pos.x - collision.box.width / 2.0f;
                    collision.box.y = transform.pos.y - collision.box.height / 2.0f;
                }
            }
            
            current_rec = rec;

            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) mouseMoveRect = false;

            if (gCoordinator.HasComponent<box_render>(current_en))
            {
                if (IsKeyPressed(KEY_SPACE))
                {
                    auto& transform = gCoordinator.GetComponent<transform2D>(current_en);
                    auto& stats = gCoordinator.GetComponent<status>(current_en);
                    auto& box = gCoordinator.GetComponent<box_render>(current_en);
                    auto& col = gCoordinator.GetComponent<collidble>(current_en);

                    int copy = gCoordinator.CreateEntity();

                    gCoordinator.AddComponent(copy,
                        status{ stats.active, true, WALL });

                    gCoordinator.AddComponent(copy,
                        transform2D{ transform.pos });

                    gCoordinator.AddComponent(copy,
                        box_render{ box.dimensions, box.platform });

                    gCoordinator.AddComponent(copy,
                        col.box);
                }
            }

            if (gCoordinator.HasComponent<render_environment>(current_en))
            {
                auto& rend = gCoordinator.GetComponent<render_environment>(current_en);
                auto& stats = gCoordinator.GetComponent<status>(current_en);
                ASSETS texture = rend.txt;

                if (IsKeyPressed(KEY_LEFT))
                {
                    if (ROOM15 < rend.txt)
                    {
                        rend.txt = (ASSETS)((int)rend.txt - 1);
                        if (rend.txt == ROOM15)
                        {
                            rend.txt = BG_GLASS;
                        }
                        if (rend.txt == CREDITS_BUTTON)
                        {
                            rend.txt = VINE4;
                        }
                    }
                }
                if (IsKeyPressed(KEY_RIGHT))
                {
                    if (ROOM15 < rend.txt)
                    {
                        rend.txt = (ASSETS)((int)rend.txt + 1);

                        if (rend.txt == FRUIT1)
                        {
                            rend.txt = BG_GLASS;
                        }
                        if (rend.txt == COUNT)
                        {
                            rend.txt = BRANCH1;
                        }
                    }
                }
                if (IsKeyPressed(KEY_DOWN))
                {
                    rend.depth -= 1;
                    stats.dirty = true;
                }
                if (IsKeyPressed(KEY_UP))
                {
                    rend.depth += 1;
                    stats.dirty = true;
                }
                if (rend.depth < 0)
                {
                    rend.depth = 0;
                }

                if (IsKeyPressed(KEY_C))
                {
                    rend.flip_hor = !rend.flip_hor;
                }
                if (IsKeyPressed(KEY_Z))
                {
                    rend.flip_ver = !rend.flip_ver;
                }
                if (IsKeyDown(KEY_X))
                {
                    rend.rotation += 5.0f;
                }
                if (IsKeyDown(KEY_V))
                {
                    rend.rotation -= 5.0f;
                }

                if (IsKeyPressed(KEY_SPACE))
                {
                    auto& transform = gCoordinator.GetComponent<transform2D>(current_en);
                    Texture2D txt = gAssetMngr.GetAsset(rend.txt);
                    int copy = gCoordinator.CreateEntity();

                    gCoordinator.AddComponent(copy,
                        status{ stats.active, true, ENVIRONMENT });

                    gCoordinator.AddComponent(copy,
                        transform2D{ transform.pos});

                    gCoordinator.AddComponent(copy,
                        render_environment{ rend.animate, rend.flip_ver, rend.flip_hor, 
                        rend.txt, rend.depth, rend.size, rend.rotation });
                }

                if (IsKeyPressed(KEY_LEFT_BRACKET))
                {
                    rend.size += 0.5;
                }
                if (IsKeyPressed(KEY_RIGHT_BRACKET))
                {
                    rend.size -= 0.5;
                }

                if (rend.size <= 0)
                {
                    rend.size = 0.25;
                }
            }
        }
        check_inputs();
    }
}

void Tooling::init()
{
    mouseMoveRect = false;
    mouseScaleMode = false;
    mouseScaleReady = false;
    tooling_on = false;
}

void Tooling::draw()
{
    if (mouseScaleReady)
    {
        DrawRectangleLinesEx(current_rec, 2, RED);
        DrawTriangle(
            Vector2{ current_rec.x + current_rec.width - MOUSE_SCALE_MARK_SIZE, current_rec.y + current_rec.height},
            Vector2{ current_rec.x + current_rec.width, current_rec.y + current_rec.height},
            Vector2{ current_rec.x + current_rec.width, current_rec.y + current_rec.height - MOUSE_SCALE_MARK_SIZE},
            RED);
    }

    if (mouseMoveRect)
    {
        DrawRectangleRec(current_rec, ColorAlpha(PINK, 0.5));
    }
}

void Tooling::check_inputs()
{
    if (IsKeyPressed(KEY_ZERO))
    {
        serialize();
    }

    if (IsKeyPressed(KEY_THREE))
    {
        Vector2 mouse_pos = GetScreenToWorld2D(GetMousePosition(), gCamera);

        int en = gCoordinator.CreateEntity();

        gCoordinator.AddComponent(en,
            status{ true, true, WALL });

        gCoordinator.AddComponent(en,
            box_render{ 50, 50 });

        gCoordinator.AddComponent(en,
            transform2D{ mouse_pos.x, mouse_pos.y });

         gCoordinator.AddComponent(
         en,
         collidble{ Rectangle{mouse_pos.x, mouse_pos.y, 50, 50 } });

            current_rec = Rectangle{ mouse_pos.x, mouse_pos.y, 500, 500 };
            current_en = en;
    }

    if (IsKeyPressed(KEY_FOUR))
    {
        Vector2 mouse_pos = GetScreenToWorld2D(GetMousePosition(), gCamera);

        int en = gCoordinator.CreateEntity();

        gCoordinator.AddComponent(en,
            status{ true, true, ENVIRONMENT });

        gCoordinator.AddComponent(en,
            transform2D{ mouse_pos.x, mouse_pos.y });

        gCoordinator.AddComponent(en,
            render_environment{ true, false, false, BRANCH1, 0, 1.0f, 0.0f });

        //gCoordinator.AddComponent(en,
        //    animate{ 3.0f, 0, FRUIT2});

        //gCoordinator.AddComponent(en,
        //    collidble{ Rectangle{mouse_pos.x, mouse_pos.y, 200, 300 } });

        //gCoordinator.AddComponent(en,
        //    collectable{ false });

            current_rec = Rectangle{ mouse_pos.x, mouse_pos.y, 500, 500 };
            current_en = en;
    }

    if (IsKeyPressed(KEY_FIVE))
    {
        Vector2 mouse_pos = GetScreenToWorld2D(GetMousePosition(), gCamera);

        int en = gCoordinator.CreateEntity();

        gCoordinator.AddComponent(en,
            status{ true, true, ENVIRONMENT });

        gCoordinator.AddComponent(en,
            transform2D{ mouse_pos.x, mouse_pos.y });

        gCoordinator.AddComponent(en,
            render_environment{ false, false, false, BRANCH1, 0, 1.0f, 0.0f });

        //gCoordinator.AddComponent(en,
        //    box_render{ Vector2{100, 100}, false });

        //gCoordinator.AddComponent(en,
        //    collidble{ Rectangle{mouse_pos.x, mouse_pos.y, 100, 100 } });

        //gCoordinator.AddComponent(en,
        //    collecting{ FRUIT, 2 });

        current_rec = Rectangle{ mouse_pos.x, mouse_pos.y, 500, 500 };
        current_en = en;
    }

    if (IsKeyPressed(KEY_BACKSPACE))
    {
        auto& stats = gCoordinator.GetComponent<status>(current_en);

        stats.active = false;
    }

    if (IsKeyPressed(KEY_E))
    {
        only_env = !only_env;
    }

    if (IsKeyPressed(KEY_T))
    {
        only_walls = !only_walls;
    }

    //static size_t count = 0;
    //if (IsKeyPressed(KEY_EIGHT))
    //{
    //    Vector2 mouse_pos = GetScreenToWorld2D(GetMousePosition(), gCamera);

    //    int en = gCoordinator.CreateEntity();

    //    gCoordinator.AddComponent(en,
    //        status{ true, true, WAYPOINT });

    //    gCoordinator.AddComponent(en,
    //        transform2D{ mouse_pos.x, mouse_pos.y });

    //    gCoordinator.AddComponent(en,
    //        box_render{ 50, 50 });

    //    gCoordinator.AddComponent(en,
    //        waypoint{ count++ });

    //    float size = 750.0f;

    //    gCoordinator.AddComponent(
    //        en,
    //        collidble{ Rectangle{mouse_pos.x - size / 2.0f,
    //        mouse_pos.y - size / 2.0f,
    //            size, size } });

    //    current_rec = Rectangle{ mouse_pos.x, mouse_pos.y, 500, 500 };
    //    current_en = en;
    //}
    //if (IsKeyPressed(KEY_NINE))
    //{
    //    Vector2 mouse_pos = GetScreenToWorld2D(GetMousePosition(), gCamera);

    //    int en = gCoordinator.CreateEntity();

    //    gCoordinator.AddComponent(en,
    //        status{ true, true, WAYPOINT });

    //    gCoordinator.AddComponent(en,
    //        transform2D{ mouse_pos.x, mouse_pos.y });

    //    gCoordinator.AddComponent(en,
    //        box_render{ 50, 50 });

    //    gCoordinator.AddComponent(en,
    //        waypoint{ count++, false });

    //    float size = 750.0f;

    //    gCoordinator.AddComponent(
    //        en,
    //        collidble{ Rectangle{mouse_pos.x - size / 2.0f,
    //        mouse_pos.y - size / 2.0f,
    //            size, size } });

    //    current_rec = Rectangle{ mouse_pos.x, mouse_pos.y, 500, 500 };
    //    current_en = en;
    //}
    //if (IsKeyPressed(KEY_NINE))
    //{
    //    Vector2 mouse_pos = GetScreenToWorld2D(GetMousePosition(), gCamera);

    //    int en = gCoordinator.CreateEntity();

    //    float size = 100.0f;

    //    gCoordinator.AddComponent(en,
    //        status{ true, true, ENDPOINT });

    //    gCoordinator.AddComponent(en,
    //        transform2D{ mouse_pos.x, mouse_pos.y });

    //    gCoordinator.AddComponent(en,
    //        box_render{ size, size });

    //    gCoordinator.AddComponent(
    //        en,
    //        collidble{ Rectangle{mouse_pos.x,
    //        mouse_pos.y, size, size} });

    //    current_rec = Rectangle{ mouse_pos.x, mouse_pos.y, size, size };
    //    current_en = en;
    //}
}

void Tooling::delete_inactivity()
{
    std::vector<Entity> deleting_list;

    for (auto& entity : entities_list)
    {
        auto const& stats = gCoordinator.GetComponent<status>(entity);
        if (!stats.active)
        {
            deleting_list.push_back(entity);
        }
    }

    for (int i = 0; i < deleting_list.size(); i++)
    {
        gCoordinator.DestroyEntity(deleting_list[i]);
    }
}