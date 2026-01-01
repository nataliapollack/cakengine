#pragma once
#include "raylib.h"
#include "Game.h"

// future of this is gonna be threaded btw.
class AssetManager
{
public:
    void load_assets();

    Texture2D GetAsset(int asset);
    Vector2 GetDimensions(int asset, float multiplier);

    void unload();

private:
    Texture2D textures[COUNT];
};