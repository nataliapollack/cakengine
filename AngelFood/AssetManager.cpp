#include "AssetManager.h"
//#include <chrono>
//#include <iostream>

void AssetManager::load_assets()
{
    //using namespace std::chrono;

    //auto start = high_resolution_clock::now();
    // playerr
    textures[HALO] = LoadTexture("art/HALO.png");
    textures[PLAYER_IDLE] = LoadTexture("art/AngelSprite/Idle.png");
    textures[PLAYER_FALL] = LoadTexture("art/AngelSprite/Fall.png");
    textures[PLAYER_JUMP] = LoadTexture("art/AngelSprite/Jump.png");
    //auto stop = high_resolution_clock::now();

    //auto duration = duration_cast<microseconds>(stop - start);
    //std::cout << duration.count() << "\n";

    textures[PLAYER_GLIDE] = LoadTexture("art/AngelSprite/Glide.png");
    textures[PLAYER_WALK1] = LoadTexture("art/AngelSprite/Walk1.png");
    textures[PLAYER_WALK2] = LoadTexture("art/AngelSprite/Walk2.png");
    textures[PLAYER_HURT] = LoadTexture("art/AngelSprite/hurt.png");
    textures[PLAYER_DEAD] = LoadTexture("art/AngelSprite/dead.png");

    textures[DAEDALUS_IDLE] = LoadTexture("art/placeholderDaedalus.png");
    textures[DAEDALUS_FLOAT] = LoadTexture("art/DaedalusFloat.png");

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
    textures[ROOM12] = LoadTexture("art/room12.png");
    textures[ROOM13] = LoadTexture("art/room13.png");
    textures[ROOM14] = LoadTexture("art/room14.png");
    textures[ROOM15] = LoadTexture("art/room15.png");

    // textures
    textures[BRANCH1] = LoadTexture("art/Tiles Set 1/Floating Branch 1.png");
    textures[BRANCH2] = LoadTexture("art/Tiles Set 1/Floating Branch 2.png");
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
    textures[ROOTS] = LoadTexture("art/Tiles Set 2/Deco Roots.png");

    textures[BG_BRANCH] = LoadTexture("art/Tiles Set 3/BG Branch 1.png");
    textures[BG_BUSH1] = LoadTexture("art/Tiles Set 3/BG Bush 1.png");
    textures[BG_BUSH2] = LoadTexture("art/Tiles Set 3/BG Bush 2.png");
    textures[BG_VINE1] = LoadTexture("art/Tiles Set 3/BG Vine 1.png");
    textures[BG_VINE2] = LoadTexture("art/Tiles Set 3/BG Vine 2.png");
    textures[BG_ROOT1] = LoadTexture("art/Tiles Set 3/Big Root 1.png");
    textures[BG_ROOT2] = LoadTexture("art/Tiles Set 3/Big Root 2.png");
    textures[BG_ROOT3] = LoadTexture("art/Tiles Set 3/Big Root 3.png");
    textures[BG_BUSH3] = LoadTexture("art/Tiles Set 3/Bush 4.png");
    textures[BG_BUSH4] = LoadTexture("art/Tiles Set 3/Bush 5.png");
    textures[BG_BUSH5] = LoadTexture("art/Tiles Set 3/Bush 6.png");
    textures[BG_BUSH6] = LoadTexture("art/Tiles Set 3/Bush 7.png");
    textures[BG_BUSH7] = LoadTexture("art/Tiles Set 3/Bush 8.png");
    textures[BG_BUSH8] = LoadTexture("art/Tiles Set 3/Bush 9.png");
    textures[HANGING1] = LoadTexture("art/Tiles Set 3/Hanging 1.png");
    textures[HANGING2] = LoadTexture("art/Tiles Set 3/Hanging 2.png");
    textures[HANGING3] = LoadTexture("art/Tiles Set 3/Hanging 3.png");
    textures[HANGING4] = LoadTexture("art/Tiles Set 3/Hanging 4.png");
    textures[VINE1] = LoadTexture("art/Tiles Set 3/Vine 1.png");
    textures[VINE2] = LoadTexture("art/Tiles Set 3/Vine 2.png");
    textures[VINE3] = LoadTexture("art/Tiles Set 3/Vine 3.png");
    textures[VINE4] = LoadTexture("art/Tiles Set 3/Vine 4.png");
    textures[BG_GLASS] = LoadTexture("art/Tiles Set 3/Glass.png");

    // MORE Animations
    textures[FRUIT1] = LoadTexture("art/fruit_0000.png");
    textures[FRUIT2]= LoadTexture("art/fruit_0001.png");
    textures[KING_HAPPY1] = LoadTexture("art/bird_happy_0000.png");
    textures[KING_HAPPY2] = LoadTexture("art/bird_happy_0001.png");
    textures[BIRD_HAPPY1] = LoadTexture("art/minibird_happy_0000.png");
    textures[BIRD_HAPPY2] = LoadTexture("art/minibird_happy_0001.png");
    textures[SHEEP_HAPPY1] = LoadTexture("art/sheep_happy_0000.png");
    textures[SHEEP_HAPPY2] = LoadTexture("art/sheep_happy_0001.png");
    textures[SNAKE_PLAY1] = LoadTexture("art/snake_Playing_0000.png");
    textures[SNAKE_PLAY2] = LoadTexture("art/snake_Playing_0001.png");
    textures[SPARK1] = LoadTexture("art/spark_0000.png");
    textures[SPARK2] = LoadTexture("art/spark_0001.png");
    textures[SPARK3] = LoadTexture("art/spark_0002.png");
    textures[SPARK4] = LoadTexture("art/spark_0003.png");

    // Main menu stuff
    textures[TITLE] = LoadTexture("art/MainMenu/Title.png");
    textures[MENU_ILLUS1] = LoadTexture("art/MainMenu/Menu Illus.png");
    textures[MENU_ILLUS2] = LoadTexture("art/MainMenu/Menu Illus2.png");
    textures[START_BUTTON] = LoadTexture("art/MainMenu/Start.png");
    textures[CREDITS_BUTTON] = LoadTexture("art/MainMenu/Credits.png");

    textures[COVER] = LoadTexture("art/HiddenAreaBarrier.png");

    // MORE ART
    textures[BIRDHOUSE] = LoadTexture("art/birdhouses.png");
    textures[FARM1] = LoadTexture("art/farm_back.png");
    textures[FARM2] = LoadTexture("art/farm_fence.png");
    textures[FARM3] = LoadTexture("art/farm_front.png");
    textures[FINAL_ROOM] = LoadTexture("art/level2.png");

    // custscene stuff
    textures[CUTSCENE1] = LoadTexture("art/Cutscene/cutscene1.png");
    textures[CUTSCENE2] = LoadTexture("art/Cutscene/cutscene2.png");
    textures[CUTSCENE3] = LoadTexture("art/Cutscene/cutsceneEnd.png");
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
