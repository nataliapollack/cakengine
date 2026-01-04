
#include "MainMenu.h"
#include "AssetManager.h"
#include "Coordinator.hpp"

#include "raymath.h"

extern Coordinator gCoordinator;
extern AssetManager gAssetMngr;

void MainMenu::init()
{
    title = gAssetMngr.GetAsset(TITLE);
    illus1 = gAssetMngr.GetAsset(MENU_ILLUS1);
    illus2 = gAssetMngr.GetAsset(MENU_ILLUS2);
    start = gAssetMngr.GetAsset(START_BUTTON);
    credits = gAssetMngr.GetAsset(CREDITS_BUTTON);

    cut1 = gAssetMngr.GetAsset(CUTSCENE1);
    cut2 = gAssetMngr.GetAsset(CUTSCENE2);

    outro_time = Timer(3.0f);

    fade_time = Timer(1.0f);
    gap_time = Timer(0.5f);
    illus_time = Timer(1.0f);
    start_time = Timer(1.0f);

    curr_button = Button::START;
    curr_scene = Scene::MAINMENU;

    start_game = false;
    change_illus = false;
    outro_running = false;

    gCoordinator.AddEventListener(
        METHOD_LISTENER(Events::Collision::ENDPOINT,
            MainMenu::SetOutro
        ));
}

void MainMenu::updateIntro(float dt)
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

void MainMenu::drawIntro()
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

void MainMenu::updateOutro(float dt)
{
    // do stuff
    if (outro_time.update(dt))
    {
        // exit

        // fade menu back in
        curr_scene = Scene::MAINMENU;
        outro_running = false;

        start_game = false;
        change_illus = false;
        curr_button = Button::START;
        fade_time.reset();
        start_time.reset();

        Event reset(Events::Scene::RESET);
        gCoordinator.SendEvent(reset);
    }
}

void MainMenu::drawOutro()
{
    // DRAW
    ClearBackground({ 9, 10, 15, 255 });
    BeginDrawing();

    // Outro cutscenes

    // cutscene 1
    float percent = (outro_time.count() / outro_time.time());
    if (percent < 0.5f)
    {
        Rectangle source{ 0, 0, cut1.width, cut1.height };

        Rectangle dest{ 0.0f, 0.0f, cut1.width, cut1.height };

        DrawTexturePro(cut1, source, dest,
            Vector2Zero(), 0.0f, ColorAlpha(WHITE, 1.0f));
    }
    else
    {
        float fade = (percent > 0.9f) ? 10.0f * (1.0f - percent) : 1.0f;

        Rectangle source{ 0, 0, cut2.width, cut2.height };

        Rectangle dest{ 0.0f, 0.0f, cut2.width, cut2.height };

        DrawTexturePro(cut2, source, dest,
            Vector2Zero(), 0.0f, ColorAlpha(WHITE, fade));
    }

    EndDrawing();
}

Scene MainMenu::GetScene() const
{
    return curr_scene;
}

void MainMenu::SetScene(Scene newScene)
{
    curr_scene = newScene;
}

void MainMenu::SetOutro(Event& event)
{
    outro_running = true;

    outro_time.start();
}

bool MainMenu::GetOutro() const
{
    return outro_running;
}
