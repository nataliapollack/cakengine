#include "AssetManager.h"

void AssetManager::load_assets()
{
    // playerr
    textures[HALO] = LoadTexture("art/player/HALO.png");
    textures[PLAYER_IDLE] = LoadTexture("art/player/PLAYER_IDLE.png");
    textures[PLAYER_FALL] = LoadTexture("art/player/PLAYER_FALL.png");
    textures[TEMP_ITEM] = LoadTexture("art/TEMP_ITEM.png");
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