
#include "MainMenu.h"
#include "AssetManager.h"

#include "raymath.h"

extern AssetManager gAssetMngr;

void MainMenu::init()
{
    title = gAssetMngr.GetAsset(TITLE);
    illus1 = gAssetMngr.GetAsset(MENU_ILLUS1);
    illus2 = gAssetMngr.GetAsset(MENU_ILLUS2);
    start = gAssetMngr.GetAsset(START_BUTTON);
    credits = gAssetMngr.GetAsset(CREDITS_BUTTON);

    fade_time = Timer(1.0f);
    gap_time = Timer(0.5f);
    illus_time = Timer(1.0f);
    start_time = Timer(1.0f);

    curr_button = Button::START;
    curr_scene = Scene::MAINMENU;

    start_game = false;
    change_illus = false;
}

void MainMenu::update(float dt)
{
    if (curr_button == Button::START && !start_game)
    {
        if (IsKeyPressed(KEY_RIGHT))
        {
            curr_button = Button::CREDITS;
        }
        if (IsKeyReleased(KEY_ENTER))
        {
            start_game = true;
            fade_time.start();
        }
    }

    if (curr_button == Button::CREDITS && !start_game)
    {
        if (IsKeyPressed(KEY_LEFT))
        {
            curr_button = Button::START;
        }
    }

    if (start_game)
    {
        if (fade_time.update(dt))
        {
            gap_time.start();
        }
        if (gap_time.update(dt))
        {
            change_illus = true;
            illus_time.start();
        }
        if (illus_time.update(dt))
        {
            start_time.start();
        }
        if (start_time.update(dt))
        {
            curr_scene = Scene::GAMEPLAY;
        }
    }
}

void MainMenu::draw()
{
    float fade = 1.0f -
        ((fade_time.count() / fade_time.time())
            * 1.0f);

    float illus_fade = 1.0f -
        ((start_time.count() / start_time.time()) * 1.0f);

    // DRAW
    ClearBackground({ 9, 10, 15, 255 });
    BeginDrawing();

    float upper_border = GetScreenHeight() * 0.1f;
    float width = GetScreenWidth();
    float scale = 0.8f;

    {
        float title_width = title.width * scale;
        float title_height = title.height * scale;

        Rectangle source{ 0, 0, title.width, title.height };

        Rectangle dest{ (width - title_width) * 0.5f, upper_border,
            title_width, title_height };

        DrawTexturePro(title, source, dest,
            Vector2Zero(), 0.0f, ColorAlpha(WHITE, fade));
    }

    {
        Texture2D illus =
            (change_illus) ? illus2 : illus1;

        float illus_width = illus.width * scale;
        float illus_height = illus.height * scale;

        Rectangle source{ 0, 0,
            illus.width, illus.height };

        Rectangle dest{
            (width - illus_width) * 0.52f,
            upper_border + title.height * 0.8f * scale,
            illus_width, illus_height };

        DrawTexturePro(
            illus,
            source, dest,
            Vector2Zero(), 0.0f, ColorAlpha(WHITE, illus_fade));
    }

    {
        float select_scale =
            (curr_button == Button::START) ? 1.5f : 1.0f;

        select_scale = (curr_button == Button::START &&
            IsKeyDown(KEY_ENTER)) ? 1.25 : select_scale;

        float start_width = start.width * scale * scale *
            select_scale;
        float start_height = start.height * scale * scale *
            select_scale;

        Rectangle source{ 0, 0,
            start.width, start.height };

        Rectangle dest{
            (width - start_width) * 0.2f,
            upper_border + title.height * 0.8f * scale +
            illus1.height * scale * 1.2,
            start_width, start_height };

        Color color = (curr_button == Button::START) ?
            ColorAlpha(IsKeyDown(KEY_ENTER) ? BLUE : SKYBLUE, fade) :
            ColorAlpha(WHITE, fade);

        DrawTexturePro(start, source, dest,
            Vector2Zero(), 0.0f,
            color);
    }

    {
        float select_scale =
            (curr_button == Button::CREDITS) ? 1.5f : 1.0f;

        select_scale = (curr_button == Button::CREDITS &&
            IsKeyDown(KEY_ENTER)) ? 1.25 : select_scale;

        float credits_width = credits.width * scale * scale *
            select_scale;
        float credits_height = credits.height * scale * scale *
            select_scale;

        Rectangle source{ 0, 0,
            credits.width, credits.height };

        Rectangle dest{
            (width - credits_width) * 0.8f,
            upper_border + title.height * 0.8f * scale +
            illus1.height * scale * 1.2,
            credits_width, credits_height };

        Color color = (curr_button == Button::CREDITS) ?
            ColorAlpha(IsKeyDown(KEY_ENTER) ? BLUE : SKYBLUE, fade) :
            ColorAlpha(WHITE, fade);

        DrawTexturePro(credits, source, dest,
            Vector2Zero(), 0.0f,
            color);
    }

    EndDrawing();
}

Scene MainMenu::GetScene() const
{
    return curr_scene;
}
