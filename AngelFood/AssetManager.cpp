#include "AssetManager.h"

void AssetManager::load_assets()
{
    // playerr
    textures[HALO] = LoadTexture("art/HALO.png");
    textures[PLAYER_IDLE] = LoadTexture("art/PLAYER_IDLE.png");
    textures[PLAYER_FALL] = LoadTexture("art/PLAYER_FALL.png");


    textures[DAEDALUS_IDLE] = LoadTexture("art/placeholderDaedalus.png");

    textures[KING_IDLE1] = LoadTexture("art/birdNPC_0000.png");
    textures[KING_IDLE2] = LoadTexture("art/birdNPC_0001.png");

    textures[BIRD_IDLE1] = LoadTexture("art/minibird_0000.png");
    textures[BIRD_IDLE2] = LoadTexture("art/minibird_0001.png");

    textures[SNAKE_IDLE1] = LoadTexture("art/snake_Idle_0000.png");
    textures[SNAKE_IDLE2] = LoadTexture("art/snake_Idle_0001.png");

    textures[SHEEP_IDLE1] = LoadTexture("art/sheep_0000.png");
    textures[SHEEP_IDLE2] = LoadTexture("art/sheep_0001.png");

    textures[ROOM1] = LoadTexture("art/BG1 Test.png");
    textures[ROOM5] = LoadTexture("art/room5.png");
    textures[ROOM6] = LoadTexture("art/room6.png");
    textures[ROOM7] = LoadTexture("art/room7.png");
    textures[ROOM7] = LoadTexture("art/room7.png");
    textures[ROOM8] = LoadTexture("art/room8.png");
    textures[ROOM9] = LoadTexture("art/room9.png");
    textures[ROOM10] = LoadTexture("art/room10.png");
    textures[ROOM11] = LoadTexture("art/room11.png");
}

Texture2D AssetManager::GetAsset(int asset)
{
    return textures[asset];
}

Vector2 AssetManager::GetDimensions(int asset, float mulitplier)
{
    return Vector2{ (float)textures[asset].width * mulitplier, (float)textures[asset].height * mulitplier };
}

void AssetManager::unload()
{
    for (int i = 0; i < COUNT; i++)
    {
        UnloadTexture(textures[i]);
    }
}