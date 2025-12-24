#include "AssetManager.h"

void AssetManager::load_assets()
{
    textures[0] = LoadTexture("art/0.png");
    textures[1] = LoadTexture("art/1.png");
    textures[2] = LoadTexture("art/temp.png");
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