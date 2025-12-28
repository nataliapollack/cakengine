#include "Tooling.h"
#include "Game.h"
#include "Coordinator.hpp"

// components
#include "Core.h"
#include "Player.h"
#include "Progression.h"

#include <iostream>
#include <string>
#include <fstream>
#include "raymath.h"


#define MOUSE_SCALE_MARK_SIZE   12

extern Coordinator gCoordinator;
extern Camera2D gCamera;

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

                gCoordinator.AddComponent(en,
                    box_render{ x, y });

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
                if (!gCoordinator.HasComponent<box_render>(entity))
                    continue;

                Vector2 mouse_pos = GetScreenToWorld2D(GetMousePosition(), gCamera);

                {
                    auto& collision = gCoordinator.GetComponent<collidble>(entity);
                    Rectangle rec = collision.box;
                    Rectangle rec2 = { rec.x + rec.width - MOUSE_SCALE_MARK_SIZE, rec.y + rec.height - MOUSE_SCALE_MARK_SIZE, MOUSE_SCALE_MARK_SIZE, MOUSE_SCALE_MARK_SIZE };


                    if (CheckCollisionPointRec(mouse_pos, rec2))
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
                        if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
                        {
                            mouseMoveRect = true;

                            mOffset = Vector2Subtract(Vector2{ rec.x, rec.y }, mouse_pos);
                            continue;
                        }
                    }
                }
            }
        }

        if (mouseScaleMode)
        {
            Vector2 mouse_pos = GetScreenToWorld2D(GetMousePosition(), gCamera);

            auto& collision = gCoordinator.GetComponent<collidble>(current_en);
            auto& render = gCoordinator.GetComponent<box_render>(current_en);

            Rectangle rec = collision.box;

            mouseScaleReady = true;

            rec.width = (mouse_pos.x - rec.x);
            rec.height = (mouse_pos.y - rec.y);

            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) mouseScaleMode = false;

            collision.box = rec;
            current_rec = collision.box;

            render.dimensions.x = collision.box.width;
            render.dimensions.y = collision.box.height;
        }

        if (mouseMoveRect)
        {
            Vector2 mouse_pos = GetScreenToWorld2D(GetMousePosition(), gCamera);

            auto& collision = gCoordinator.GetComponent<collidble>(current_en);
            auto& transform = gCoordinator.GetComponent<transform2D>(current_en);

            transform.pos = Vector2Add(mouse_pos, mOffset);
            collision.box.x = transform.pos.x;
            collision.box.y = transform.pos.y;
            current_rec = collision.box;

            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT)) mouseMoveRect = false;

            if (IsKeyPressed(KEY_Z))
            {
                gCoordinator.DestroyEntity(current_en);
                mouseMoveRect = false;
                return;
            }

        }

        if (IsKeyPressed(KEY_ZERO))
        {
            serialize();
        }

        if (IsKeyPressed(KEY_FIVE))
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

             current_rec = Rectangle{ mouse_pos.x, mouse_pos.y, 50, 50 };
             current_en = en;
        }
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