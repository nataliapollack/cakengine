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

    textures[ROOM1] = LoadTexture("art/room1.png");
    textures[ROOM2] = LoadTexture("art/room2.png");
    textures[ROOM3] = LoadTexture("art/room3.png");
    textures[ROOM4] = LoadTexture("art/room4.png");
    textures[ROOM5] = LoadTexture("art/room5.png");
    textures[ROOM6] = LoadTexture("art/room6.png");
    textures[ROOM7] = LoadTexture("art/room7.png");
    textures[ROOM8] = LoadTexture("art/room8.png");
    textures[ROOM9] = LoadTexture("art/room9.png");
    textures[ROOM10] = LoadTexture("art/room10.png");
    textures[ROOM11] = LoadTexture("art/room11.png");

    textures[ROOM12] = LoadTexture("art/TEMP_ITEM.png");
    textures[ROOM13] = LoadTexture("art/TEMP_ITEM.png");

    textures[ROOM14] = LoadTexture("art/room14.png");
    textures[ROOM15] = LoadTexture("art/room15.png");

    // textures
    textures[BRANCH1] = LoadTexture("art/Tiles Set 1/Floating Branch1.png");
    textures[BRANCH2] = LoadTexture("art/Tiles Set 1/Floating Branch2.png");
    textures[FLOATING1] = LoadTexture("art/Tiles Set 1/Floating 1.png");
    textures[FLOATING2] = LoadTexture("art/Tiles Set 1/Floating 2.png");
    textures[EDGE1] = LoadTexture("art/Tiles Set 1/Edge Grass 1.png");
    textures[EDGE2] = LoadTexture("art/Tiles Set 1/Edge Grass 2.png");
    textures[EDGE3] = LoadTexture("art/Tiles Set 1/Edge Grass 2.png");
    textures[GROUND1] = LoadTexture("art/Tiles Set 1/GroundGrass 1.png");
    textures[GROUND2] = LoadTexture("art/Tiles Set 1/Ground Grass 2.png");
    textures[DARK] = LoadTexture("art/Tiles Set 1/1x1.png");
    textures[SIDE1] = LoadTexture("art/Tiles Set 1/Side Piece 1.png");
    textures[SIDE2] = LoadTexture("art/Tiles Set 1/Side Piece 2.png");
    textures[SIDE3] = LoadTexture("art/Tiles Set 1/Side Piece 3.png");
    textures[GLASS1] = LoadTexture("art/Tiles Set 1/Glass 1.png");
    textures[GLASS2] = LoadTexture("art/Tiles Set 1/Glass 2.png");
    textures[GLASS3] = LoadTexture("art/Tiles Set 1/Glass 3.png");

    // ENVIRNOMETN art
    textures[BUSH1] = LoadTexture("art/Tiles Set 2/Bush 1.png");
    textures[BUSH2] = LoadTexture("art/Tiles Set 2/Bush 2.png");
    textures[BUSH3] = LoadTexture("art/Tiles Set 2/Bush 3.png");
    textures[ENV_BRANCH1] = LoadTexture("art/Tiles Set 2/Environ Branch 1.png");
    textures[ENV_BRANCH2] = LoadTexture("art/Tiles Set 2/Environ Branch 2.png");
    textures[ENV_BRANCH3] = LoadTexture("art/Tiles Set 2/Environ Branch Large 3.png");
    textures[ENV_BRANCH4] = LoadTexture("art/Tiles Set 2/Environ Branch Large 4.png");
    textures[GRASS1] = LoadTexture("art/Tiles Set 2/Grass1.png");
    textures[GRASS2] = LoadTexture("art/Tiles Set 2/Grass2.png");
    textures[GRASS3] = LoadTexture("art/Tiles Set 2/Grass3.png");
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
