#include "Coordinator.hpp"

#include "managers/LevelMngr.h"

#include "components/Player.h"
#include "components/Render.h"
#include "components/Transform.h"
#include "components/Background.h"
#include "components/Collidble.h"

#include "systems/RenderSystem.h"
#include "systems/PlayerMovementSystem.h"
#include "systems/CollisionSystem.h"

#include "Game.h"

extern Coordinator gCoordinator;

void LevelManager::init()
{
    gCoordinator.AddEventListener(METHOD_LISTENER(Events::Transition::CHANGEROOM, LevelManager::TriggerRoomChange));
    gCoordinator.AddEventListener(METHOD_LISTENER(Events::Day::NEWDAY, LevelManager::TriggerNewDay));
    gCoordinator.AddEventListener(METHOD_LISTENER(Events::Transition::DOOROPEN, LevelManager::TriggerDoorUnlocked));

    active_entities = 0;
    day = 0;
    current_room = ROOMS::BEDROOM;

    for (int i = 0; i < 20; i++)
    {
        entities_list[i] = -1;
    }

    register_components();
    set_signatures();
}

void LevelManager::register_components()
{
    gCoordinator.RegisterComponent<Transform2D>();
    gCoordinator.RegisterComponent<Player>();
    gCoordinator.RegisterComponent<RenderBox>();
    gCoordinator.RegisterComponent<Collidible>();
    gCoordinator.RegisterComponent<Background>();
}


void LevelManager::set_signatures()
{
    Signature sig;

    sig.set(gCoordinator.GetComponentType<Player>());
    sig.set(gCoordinator.GetComponentType<Transform2D>());
    gCoordinator.SetSystemSignature<PlayerMovementSystem>(sig);

    sig.reset();

    sig.set(gCoordinator.GetComponentType<Transform2D>());
    sig.set(gCoordinator.GetComponentType<RenderBox>());
    gCoordinator.SetSystemSignature<RenderSystem>(sig);

    sig.reset();

    sig.set(gCoordinator.GetComponentType<Collidible>());
    sig.set(gCoordinator.GetComponentType<Transform2D>());
    gCoordinator.SetSystemSignature<CollisionSystem>(sig);
}

void LevelManager::PlaceObjectsRoom()
{
    // player
    entities_list[active_entities] = gCoordinator.CreateEntity();
    {
        gCoordinator.AddComponent(
            entities_list[active_entities],
            RenderBox{ 120, 150, WHITE, entity_textures[player_idle] });
        gCoordinator.AddComponent(
            entities_list[active_entities],
            Transform2D{ Vector2 {200.0f, FLOOR_Y }, 0 });
        gCoordinator.AddComponent(
            entities_list[active_entities],
            Player{});
        gCoordinator.AddComponent(
            entities_list[active_entities],
            Collidible{ OBJ_TYPES::PLAYER, Vector2{120 , 150} });
    }
    active_entities += 1;

    // background
    entities_list[active_entities] = gCoordinator.CreateEntity();
    {
        gCoordinator.AddComponent(
            entities_list[active_entities],
            RenderBox{ (float)GetScreenWidth(), (float)GetScreenHeight() - 100, DARKBLUE, entity_textures[bedroom_bg]});
        gCoordinator.AddComponent(
            entities_list[active_entities],
            Transform2D{ Vector2 {0.0f, 70.0f }, -1 });
        gCoordinator.AddComponent(
            entities_list[active_entities],
            Background{}
        );
    }
    active_entities += 1;

    // door
    entities_list[active_entities] = gCoordinator.CreateEntity();
    {
        gCoordinator.AddComponent(
            entities_list[active_entities],
            RenderBox{ 50, 50, PURPLE });
        gCoordinator.AddComponent(
            entities_list[active_entities],
            Transform2D{ Vector2 {680.0f, FLOOR_Y }, 0 });
        gCoordinator.AddComponent(
            entities_list[active_entities],
            Collidible{ OBJ_TYPES::DOOR, Vector2{200, 200} }
        );
    }
    active_entities += 1;
}

void LevelManager::PlaceObjectsHallway()
{
    // player
    entities_list[active_entities] = gCoordinator.CreateEntity();
    {
        gCoordinator.AddComponent(
            entities_list[active_entities],
            RenderBox{ 120, 150, PURPLE, entity_textures[player_idle]});
        gCoordinator.AddComponent(
            entities_list[active_entities],
            Transform2D{ Vector2 {200.0f, FLOOR_Y + 20}, 3 });
        gCoordinator.AddComponent(
            entities_list[active_entities],
            Player{});
        gCoordinator.AddComponent(
            entities_list[active_entities],
            Collidible{ OBJ_TYPES::PLAYER, Vector2{80 , 150} });
    }
    active_entities += 1;

    // background
    entities_list[active_entities] = gCoordinator.CreateEntity();
    {
        gCoordinator.AddComponent(
            entities_list[active_entities],
            RenderBox{ (float)entity_textures[hallway_bg].width * 0.25f, ((float)entity_textures[hallway_bg].height * 0.25f) - 50.0f, DARKGREEN, entity_textures[hallway_bg] });
        gCoordinator.AddComponent(
            entities_list[active_entities],
            Transform2D{ Vector2 {0,100}, -1 });
        gCoordinator.AddComponent(
            entities_list[active_entities],
            Background{}
        );
    }
    active_entities += 1;

    // girl1
    entities_list[active_entities] = gCoordinator.CreateEntity();
    {
        gCoordinator.AddComponent(
            entities_list[active_entities],
            RenderBox{ 100, 150, WHITE, entity_textures[girl1_chibi] });
        gCoordinator.AddComponent(
            entities_list[active_entities],
            Transform2D{ Vector2 {400.0f, FLOOR_Y }, 0 });
        gCoordinator.AddComponent(
            entities_list[active_entities],
            Collidible{ OBJ_TYPES::GIRL1, Vector2{100, 250} }
        );
    }
    active_entities += 1;

    // girl2
    entities_list[active_entities] = gCoordinator.CreateEntity();
    {
        gCoordinator.AddComponent(
            entities_list[active_entities],
            RenderBox{ 100, 150, WHITE, entity_textures[girl2_chibi] });
        gCoordinator.AddComponent(
            entities_list[active_entities],
            Transform2D{ Vector2 {640.0f, FLOOR_Y }, 0 });
        gCoordinator.AddComponent(
            entities_list[active_entities],
            Collidible{ OBJ_TYPES::GIRL2, Vector2{100, 150} }
        );
    }
    active_entities += 1;


    // girl3
    entities_list[active_entities] = gCoordinator.CreateEntity();
    {
        gCoordinator.AddComponent(
            entities_list[active_entities],
            RenderBox{ 100, 150, WHITE, entity_textures[girl3_chibi] });
        gCoordinator.AddComponent(
            entities_list[active_entities],
            Transform2D{ Vector2 {874.0f, FLOOR_Y }, 0 });
        gCoordinator.AddComponent(
            entities_list[active_entities],
            Collidible{ OBJ_TYPES::GIRL3, Vector2{100, 150} }
        );
    }
    active_entities += 1;

    //LOCKED DOOR
    entities_list[active_entities] = gCoordinator.CreateEntity();
    {
        gCoordinator.AddComponent(
            entities_list[active_entities],
            RenderBox{ 50, 50, YELLOW });
        gCoordinator.AddComponent(
            entities_list[active_entities],
            Transform2D{ Vector2 {1027.0f, FLOOR_Y }, 0 });
        gCoordinator.AddComponent(
            entities_list[active_entities],
            Collidible{ OBJ_TYPES::DOOR, Vector2{50, 50} }
        );
    }
    active_entities += 1;
}

void LevelManager::ClearEntities()
{
    for (int i = 0; i < active_entities; i++)
    {
        gCoordinator.DestroyEntity(entities_list[i]);
        entities_list[i] = -1;
    }
    active_entities = 0;
}

void LevelManager::TriggerRoomChange(Event& event)
{
    ClearEntities();

    if (current_room == BEDROOM)
    {
        if (day == 3)
        {
            PlaceAlmostLastDay();
            current_room = HALLWAY;
            Event dec_happen(Events::Transition::FADEOUT);
            gCoordinator.SendEvent(dec_happen);

            Event audio(Events::Audio::SOUND);
            audio.SetParam<int>(Events::Audio::SOUND, transition2);
            gCoordinator.SendEvent(audio);
            return;
        }
        if (day == 4)
        {
            PlaceFinalDay();
            current_room = HALLWAY;
            Event audio(Events::Audio::SOUND);
            Event dec_happen(Events::Transition::FADEOUT);
            gCoordinator.SendEvent(dec_happen);

            audio.SetParam<int>(Events::Audio::SOUND, transition2);
            gCoordinator.SendEvent(audio);
            return;
        }
        else
        {
            PlaceObjectsHallway();
        }

        current_room = HALLWAY;
        Event dec_happen(Events::Transition::FADEOUT);
        gCoordinator.SendEvent(dec_happen);

        Event audio(Events::Audio::SOUND);
        audio.SetParam<int>(Events::Audio::SOUND, transition2);
        gCoordinator.SendEvent(audio);

        return;

        // transition here
    }
    if (current_room == HALLWAY)
    {
        PlaceObjectsRoom();
        current_room = BEDROOM;
        Event dec_happen(Events::Transition::FADEOUT);
        gCoordinator.SendEvent(dec_happen);

        Event min_happen(Events::Minigame::START);
        gCoordinator.SendEvent(min_happen);

        Event audio(Events::Audio::SOUND);
        audio.SetParam<int>(Events::Audio::SOUND, transition1);
        gCoordinator.SendEvent(audio);

        return;

        // tranmsition here 2, stop music
    }
}

void LevelManager::TriggerNewDay(Event& event)
{
    day++;
}

void LevelManager::load()
{
    for (int i = 0; i < TEXTURE_LIST::count; i++)
    {
        entity_textures[i] = LoadTexture(TextFormat("art/%i.png", i));
    }
}

void LevelManager::unload()
{
    for (int i = 0; i < TEXTURE_LIST::count; i++)
    {
        UnloadTexture(entity_textures[i]);
    }
}

void LevelManager::PlaceAlmostLastDay()
{
    // player
    entities_list[active_entities] = gCoordinator.CreateEntity();
    {
        gCoordinator.AddComponent(
            entities_list[active_entities],
            RenderBox{ 120, 150, PURPLE, entity_textures[player_idle] });
        gCoordinator.AddComponent(
            entities_list[active_entities],
            Transform2D{ Vector2 {200.0f, FLOOR_Y + 20 }, 3 });
        gCoordinator.AddComponent(
            entities_list[active_entities],
            Player{});
        gCoordinator.AddComponent(
            entities_list[active_entities],
            Collidible{ OBJ_TYPES::PLAYER, Vector2{80 , 150} });
    }
    active_entities += 1;

    // background
    entities_list[active_entities] = gCoordinator.CreateEntity();
    {
        gCoordinator.AddComponent(
            entities_list[active_entities],
            RenderBox{ (float)entity_textures[hallway_bg].width * 0.25f, ((float)entity_textures[hallway_bg].height * 0.25f) - 50.0f, DARKGREEN, entity_textures[hallway_bg] });
        gCoordinator.AddComponent(
            entities_list[active_entities],
            Transform2D{ Vector2 {0,100}, 0 });
        gCoordinator.AddComponent(
            entities_list[active_entities],
            Background{}
        );
    }
    active_entities += 1;

    // girl1
    entities_list[active_entities] = gCoordinator.CreateEntity();
    {
        gCoordinator.AddComponent(
            entities_list[active_entities],
            RenderBox{ 100, 150, WHITE, entity_textures[girl1_chibi] });
        gCoordinator.AddComponent(
            entities_list[active_entities],
            Transform2D{ Vector2 {400.0f, FLOOR_Y }, 1 });
        gCoordinator.AddComponent(
            entities_list[active_entities],
            Collidible{ OBJ_TYPES::GIRL1, Vector2{100, 250} }
        );
    }
    active_entities += 1;

    // girl2
    entities_list[active_entities] = gCoordinator.CreateEntity();
    {
        gCoordinator.AddComponent(
            entities_list[active_entities],
            RenderBox{ 100, 150, WHITE, entity_textures[fake_chibi] });
        gCoordinator.AddComponent(
            entities_list[active_entities],
            Transform2D{ Vector2 {640.0f, FLOOR_Y }, 1 });
        gCoordinator.AddComponent(
            entities_list[active_entities],
            Collidible{ OBJ_TYPES::FAKE, Vector2{100, 150} }
        );
    }
    active_entities += 1;


    // girl3
    entities_list[active_entities] = gCoordinator.CreateEntity();
    {
        gCoordinator.AddComponent(
            entities_list[active_entities],
            RenderBox{ 100, 150, WHITE, entity_textures[girl3_chibi] });
        gCoordinator.AddComponent(
            entities_list[active_entities],
            Transform2D{ Vector2 {874.0f, FLOOR_Y }, 1 });
        gCoordinator.AddComponent(
            entities_list[active_entities],
            Collidible{ OBJ_TYPES::GIRL3, Vector2{100, 150} }
        );
    }
    active_entities += 1;

    //LOCKED DOOR
    entities_list[active_entities] = gCoordinator.CreateEntity();
    {
        gCoordinator.AddComponent(
            entities_list[active_entities],
            RenderBox{ 50, 50, YELLOW });
        gCoordinator.AddComponent(
            entities_list[active_entities],
            Transform2D{ Vector2 {1027.0f, FLOOR_Y }, 0 });
        gCoordinator.AddComponent(
            entities_list[active_entities],
            Collidible{ OBJ_TYPES::DOOR, Vector2{50, 50} }
        );
    }
    active_entities += 1;
}


// FIRE EXIT DOOR :(
void LevelManager::PlaceObjects()
{
    // add other background here 2
    entities_list[active_entities] = gCoordinator.CreateEntity();
    {
        gCoordinator.AddComponent(
            entities_list[active_entities],
            RenderBox{ (float)GetScreenWidth(), (float)GetScreenHeight() - 100, DARKBLUE, entity_textures[bedroom_bg] });
        gCoordinator.AddComponent(
            entities_list[active_entities],
            Transform2D{ Vector2 {70.0f, 70.0f }, 0 });
        gCoordinator.AddComponent(
            entities_list[active_entities],
            Background{}
        );
    }
    active_entities += 1;

    float updated_x = (float)GetScreenWidth() + 200;
    // camera
    entities_list[active_entities] = gCoordinator.CreateEntity();
    {
        gCoordinator.AddComponent(
            entities_list[active_entities],
            RenderBox{ 220, 280, PURPLE, entity_textures[big_cam] });
        gCoordinator.AddComponent(
            entities_list[active_entities],
            Transform2D{ Vector2 {1618, 527 }, 3 });
    }
    active_entities += 1;

    // camera 2
    entities_list[active_entities] = gCoordinator.CreateEntity();
    {
        gCoordinator.AddComponent(
            entities_list[active_entities],
            RenderBox{ 220, 250, PURPLE, entity_textures[lil_cam] });
        gCoordinator.AddComponent(
            entities_list[active_entities],
            Transform2D{ Vector2 {updated_x - 200.0f, FLOOR_Y }, 3 });
    }
    active_entities += 1;

    // light 1
    entities_list[active_entities] = gCoordinator.CreateEntity();
    {
        gCoordinator.AddComponent(
            entities_list[active_entities],
            RenderBox{ 320, 350, PURPLE, entity_textures[top_light1] });
        gCoordinator.AddComponent(
            entities_list[active_entities],
            Transform2D{ Vector2 {1478, 151 }, 3 });
    }
    active_entities += 1;

    //light 2
    entities_list[active_entities] = gCoordinator.CreateEntity();
    {
        gCoordinator.AddComponent(
            entities_list[active_entities],
            RenderBox{ 320, 350, PURPLE, entity_textures[top_light2] });
        gCoordinator.AddComponent(
            entities_list[active_entities],
            Transform2D{ Vector2 {600, 150 }, 3 });
    }
    active_entities += 1;

    // light 3
    entities_list[active_entities] = gCoordinator.CreateEntity();
    {
        gCoordinator.AddComponent(
            entities_list[active_entities],
            RenderBox{ 320, 350, PURPLE, entity_textures[top_light3] });
        gCoordinator.AddComponent(
            entities_list[active_entities],
            Transform2D{ Vector2 {943, 150 }, 3 });
    }
    active_entities += 1;
    
    // light floor
    entities_list[active_entities] = gCoordinator.CreateEntity();
    {
        gCoordinator.AddComponent(
            entities_list[active_entities],
            RenderBox{ 320, 350, PURPLE, entity_textures[light_1] });
        gCoordinator.AddComponent(
            entities_list[active_entities],
            Transform2D{ Vector2 {400, 500 }, 3 });
    }
    active_entities += 1;
    
    // light floor 2
    entities_list[active_entities] = gCoordinator.CreateEntity();
    {
        gCoordinator.AddComponent(
            entities_list[active_entities],
            RenderBox{ 320, 350, PURPLE, entity_textures[light_2] });
        gCoordinator.AddComponent(
            entities_list[active_entities],
            Transform2D{ Vector2 {1300, 500 }, 3});
    }
    active_entities += 1;
    

    // chair1
    entities_list[active_entities] = gCoordinator.CreateEntity();
    {
        gCoordinator.AddComponent(
            entities_list[active_entities],
            RenderBox{ 220, 250, PURPLE, entity_textures[chair] });
        gCoordinator.AddComponent(
            entities_list[active_entities],
            Transform2D{ Vector2 {700, 520 }, 3 });
    }
    active_entities += 1;
    
    //chair 2
    entities_list[active_entities] = gCoordinator.CreateEntity();
    {
        gCoordinator.AddComponent(
            entities_list[active_entities],
            RenderBox{ 220, 250, PURPLE, entity_textures[chair] });
        gCoordinator.AddComponent(
            entities_list[active_entities],
            Transform2D{ Vector2 {900, 520 }, 3 });
    }
    active_entities += 1;

    //LOCKED DOOR
    entities_list[active_entities] = gCoordinator.CreateEntity();
    {
        gCoordinator.AddComponent(
            entities_list[active_entities],
            RenderBox{ 150, 200, YELLOW, entity_textures[red_exit]});
        gCoordinator.AddComponent(
            entities_list[active_entities],
            Transform2D{ Vector2 {56, FLOOR_Y + 20 }, 0 });
        gCoordinator.AddComponent(
            entities_list[active_entities],
            Collidible{ OBJ_TYPES::EXITDOOR, Vector2{50, 50} }
        );
    }
    active_entities += 1;
}

void LevelManager::PlaceFinalDay()
{
    float updated_x = (float)GetScreenWidth();
    // player
    entities_list[active_entities] = gCoordinator.CreateEntity();
    {
        gCoordinator.AddComponent(
            entities_list[active_entities],
            RenderBox{ 120, 150, PURPLE, entity_textures[player_idle] });
        gCoordinator.AddComponent(
            entities_list[active_entities],
            Transform2D{ Vector2 {updated_x + 200.0f, FLOOR_Y + 20}, 3 });
        gCoordinator.AddComponent(
            entities_list[active_entities],
            Player{});
        gCoordinator.AddComponent(
            entities_list[active_entities],
            Collidible{ OBJ_TYPES::PLAYER, Vector2{80 , 150} });
    }
    active_entities += 1;

    // background
    entities_list[active_entities] = gCoordinator.CreateEntity();
    {
        gCoordinator.AddComponent(
            entities_list[active_entities],
            RenderBox{ (float)entity_textures[hallway_bg].width * 0.25f, ((float)entity_textures[hallway_bg].height * 0.25f) - 50.0f, DARKGREEN, entity_textures[hallway_bg] });
        gCoordinator.AddComponent(
            entities_list[active_entities],
            Transform2D{ Vector2 {updated_x,100}, 0 });
        gCoordinator.AddComponent(
            entities_list[active_entities],
            Background{}
        );
    }
    active_entities += 1;

    // girl1
    entities_list[active_entities] = gCoordinator.CreateEntity();
    {
        gCoordinator.AddComponent(
            entities_list[active_entities],
            RenderBox{ 100, 150, WHITE, entity_textures[girl1_chibi] });
        gCoordinator.AddComponent(
            entities_list[active_entities],
            Transform2D{ Vector2 {updated_x+400.0f, FLOOR_Y }, 1 });
        gCoordinator.AddComponent(
            entities_list[active_entities],
            Collidible{ OBJ_TYPES::GIRL1, Vector2{100, 250} }
        );
    }
    active_entities += 1;

    // girl2
    entities_list[active_entities] = gCoordinator.CreateEntity();
    {
        gCoordinator.AddComponent(
            entities_list[active_entities],
            RenderBox{ 100, 150, WHITE, entity_textures[fake_chibi] });
        gCoordinator.AddComponent(
            entities_list[active_entities],
            Transform2D{ Vector2 {updated_x+640.0f, FLOOR_Y }, 1});
        gCoordinator.AddComponent(
            entities_list[active_entities],
            Collidible{ OBJ_TYPES::FAKE, Vector2{100, 150} }
        );
    }
    active_entities += 1;


    // girl3
    entities_list[active_entities] = gCoordinator.CreateEntity();
    {
        gCoordinator.AddComponent(
            entities_list[active_entities],
            RenderBox{ 100, 150, WHITE, entity_textures[girl3_chibi] });
        gCoordinator.AddComponent(
            entities_list[active_entities],
            Transform2D{ Vector2 {updated_x+874.0f, FLOOR_Y }, 1 });
        gCoordinator.AddComponent(
            entities_list[active_entities],
            Collidible{ OBJ_TYPES::GIRL3, Vector2{100, 150} }
        );
    }
    active_entities += 1;

    //LOCKED DOOR
    entities_list[active_entities] = gCoordinator.CreateEntity();
    {
        gCoordinator.AddComponent(
            entities_list[active_entities],
            RenderBox{ 50, 50, YELLOW });
        gCoordinator.AddComponent(
            entities_list[active_entities],
            Transform2D{ Vector2 {updated_x+1027.0f, FLOOR_Y }, 0 });
        gCoordinator.AddComponent(
            entities_list[active_entities],
            Collidible{ OBJ_TYPES::LOCKEDDOOR, Vector2{50, 50} }
        );
    }
    active_entities += 1;
}

void LevelManager::TriggerDoorUnlocked(Event &event)
{
    PlaceObjects();
}