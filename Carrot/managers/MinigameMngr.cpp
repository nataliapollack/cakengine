
#include "managers/MinigameMngr.h"

#include "Coordinator.hpp"

#include "raymath.h"
#include "Game.h"
#include <stdlib.h>

extern Coordinator gCoordinator;

const char* note_text[16]
{
    "King Henry had 6 wifes! Just like how you'll have 1 one day!",
    "Sonnet 29 is said to be about hope.",
    "Remember to carry the one!",
    "Isn't it weird how you can't ever move down?",

    "Queen Elizabeth II was the longest reigning monarch!",
    "Othollo's tragic flaw was misplacing his faith.",
    "Always break down complex problems first!",
    "did you notice how the lighting is always the same..?",

    "DO N'T LISTEN TO HER",
    "Ophelia is actually a tragic character, let down from those around her.",
    "DON'T LI STEN TO HER!",
    "THE DOOR THE DOOR WILL LET YO U - ",

    "It's a rhyme! A = -b +- sqrt of b squared minus 4ac all over two a!",
    "Lady Macbeth wasn't that bad of a character!",
    "Richard I was the song of King Henry the II!",
    ""
};

void MinigameManager::init()
{
    in_minigame = false;
    journal_open = false;
    is_dragging = false;

    note_text_it = 0;

    float quarter_screen = GetScreenWidth() / 3.0f;
    float third_screen_height = GetScreenHeight() * (2.0f / 3.0f);
    zones[0] = Rectangle{ 0, 0,quarter_screen,third_screen_height};
    zones[1] = Rectangle{ quarter_screen, 0,quarter_screen, third_screen_height};
    zones[2] = Rectangle{ quarter_screen * 2, 0,quarter_screen, third_screen_height };
    zones[3] = Rectangle{ 0, third_screen_height, (float)GetScreenWidth(), GetScreenHeight() / 3.0f };

    gCoordinator.AddEventListener(METHOD_LISTENER(Events::Minigame::START, MinigameManager::TriggerMinigame));
    gCoordinator.AddEventListener(METHOD_LISTENER(Events::Minigame::END, MinigameManager::TriggerMinigameEnd));

}

void MinigameManager::update()
{
    if (in_minigame)
    {
        Vector2 mouse_pos = GetMousePosition();
        for (int i = 0; i < 4; i++)
        {
            Note note = notes[i];
            Rectangle note_rec = Rectangle{ note.position.x, note.position.y, note.size.x, note.size.y };

            if (IsMouseButtonPressed(MOUSE_BUTTON_LEFT))
            {
                if (CheckCollisionPointRec(mouse_pos, note_rec))
                {
                    is_dragging = true;
                    offset = Vector2Subtract(note.position, mouse_pos);
                    MoveNote2Top(i);
                    break;
                }
            }

            if (IsMouseButtonReleased(MOUSE_BUTTON_LEFT))
            {
                is_dragging = false;
            }

            if (is_dragging)
            {
                notes[0].position = Vector2Add(mouse_pos, offset);
            }
        }
        if (!is_dragging)
        {
            CheckNotesStatus();
        }
    }
}

void MinigameManager::draw()
{
    if (journal_open)
    {
        Rectangle source = { 0,0, journal_text.width, journal_text.height };
        Vector2 origin = { 0.0f, 0.0f };
        Rectangle dest = { 0, 0, GetScreenWidth(), GetScreenHeight()};
        DrawTexturePro(journal_text, source, dest, origin, 0.0f, WHITE);


        // pencil lines
        source = { 0, 0, (float)pencil_hor.width, (float)pencil_hor.height };
        dest = { zones[1].x + 40, zones[1].y + 50, zones[1].height- 50, 50 };
        DrawTexturePro(pencil_hor, source, dest, origin, 90.0f, WHITE);

        source = { 0, 0, (float)pencil_hor.width, (float)pencil_hor.height };
        dest = { zones[2].x + 50, zones[2].y+ 50, zones[2].height - 50, 50 };
        DrawTexturePro(pencil_hor, source, dest, origin, 90.0f, WHITE);


        source = { 0, 0, (float)pencil_ver.width, (float)pencil_ver.height };
        dest = { zones[3].x, zones[3].y - 30, zones[3].width, 50 };
        DrawTexturePro(pencil_ver, source, dest, origin, 0.0f, WHITE);

        for (int i = 0; i < 3; i++)
        {
            source = { 0,0, (float)sticker_texts[i].width, (float)sticker_texts[i].height};
            dest = { sticker_placement[i].x, sticker_placement[i].y, 100, 100};
            DrawTexturePro(sticker_texts[i], source, dest, origin, 15.0f, WHITE);
        }

        for (int i = 3; i >= 0; i--)
        {
            if (i == 0 && is_dragging)
            {
                DrawNoteShadow(i);
                DrawNoteOffset(i);
            }
            else
            {
                DrawNote(i);
            }
        }
    }
}

bool MinigameManager::GetMinigameStatus()
{
    return in_minigame;
}


void MinigameManager::PlaceNotes()
{
    for (int i = 0; i < 4; i++)
    {
        Vector2 random = RandomVector(150, 300, 150, 300);
        Vector2 size = RandomVector(150, 200, 150, 200);

        AddNote(i,
            RandomVector(10, GetScreenWidth() - size.x - 10, 10, GetScreenHeight() - size.y - 10),
            size,
            NewColor(i),
            note_text[note_text_it],
            i);

        note_text_it += 1;
    }

    sticker_placement[0] = RandomVector(100, GetScreenWidth(), 100, GetScreenHeight());
    sticker_placement[1] = RandomVector(100, GetScreenWidth(), 100, GetScreenHeight());
    sticker_placement[2] = RandomVector(100, GetScreenWidth(), 100, GetScreenHeight());
}


void MinigameManager::AddNote(int i, Vector2 pos, Vector2 size, Color col, const char* txt, int zone)
{
    notes[i].color = col;
    notes[i].position = pos;
    notes[i].size = size;
    notes[i].text = txt;
    notes[i].zone = zone;
    notes[i].in_right_pos = false;
}

void MinigameManager::DrawNote(int i)
{
    Color note_color = notes[i].color;

    if (notes[i].in_right_pos)
    {
        Color correct_col= ColorAlpha(GREEN, 0.5);
        Vector2 size = Vector2{ notes[i].size.x + 20, notes[i].size.y + 20 };
        Vector2 pos = Vector2{ notes[i].position.x - 10, notes[i].position.y - 10};

        DrawRectangleV(pos, size, correct_col);

    }

    DrawRectangleV(notes[i].position, notes[i].size, note_color);


    Rectangle note_box = { notes[i].position.x + 20, notes[i].position.y, notes[i].size.x - 20, notes[i].size.y };
    DrawTextBoxedSelectable(sticky_fonts[notes[i].zone], notes[i].text, note_box, 25.0f, 1.0f, true, BLACK, 0, 0, BLACK, WHITE);
}

void MinigameManager::DrawNoteShadow(int i)
{
    DrawRectangleV(notes[i].position, notes[i].size, Fade(BLACK, 0.75f));
}

void MinigameManager::DrawNoteOffset(int i)
{
    Vector2 pos = Vector2SubtractValue(notes[i].position, 16);

    DrawRectangleV(pos, notes[i].size, notes[i].color);


    Rectangle note_box = { pos.x + 20, pos.y, notes[i].size.x - 20, notes[i].size.y };
    DrawTextBoxedSelectable(sticky_fonts[notes[i].zone], notes[i].text, note_box, 25.0f, 1.0f, true, BLACK, 0, 0, BLACK, WHITE);
}

void MinigameManager::MoveNote2Top(int index)
{
    Note copy = notes[index];

    for (int i = index; i > 0; i--)
    {
        notes[i] = notes[i - 1];
    }

    notes[0] = copy;
}

void MinigameManager::CheckNotesStatus()
{
    Rectangle box_collision;
    Rectangle box;
    int correct_count = 0;

    for (int i = 0; i < 4; i++)
    {
        box = Rectangle{ notes[i].position.x, notes[i].position.y, notes[i].size.x, notes[i].size.y};
        if (CheckCollisionRecs(box, zones[notes[i].zone]))
        {
            box_collision = GetCollisionRec(box, zones[notes[i].zone]);

            float collision_area = (int)box_collision.width * (int)box_collision.height;
            float total_area = box.width * box.height;

            if (collision_area / total_area >= 0.5)
            {
                notes[i].in_right_pos = true;
                correct_count++;
            }
            else
            {
                notes[i].in_right_pos = false;
            }
        }
    }

    if (correct_count >= 4)
    {
        // trigger next day -- if we want some extra dialogue here would be da place...
        Event day(Events::Day::NEWDAY);
        gCoordinator.SendEvent(day);
        in_minigame = false;

    }
}

void MinigameManager::TriggerMinigame(Event& event)
{
    in_minigame = true;
    journal_open = true;
    PlaceNotes();
}

void MinigameManager::TriggerMinigameEnd(Event& event)
{
    journal_open = false;
}

void MinigameManager::load()
{
    journal_text = LoadTexture("art/notebook.png");

    pencil_hor = LoadTexture("art/pencil line 1.png");
    pencil_ver = LoadTexture("art/pencil line 2.png");

    sticker_texts[0] = LoadTexture("art/duck sticker.png");
    sticker_texts[1] = LoadTexture("art/heart.png");
    sticker_texts[2] = LoadTexture("art/heart 2.png");

    sticky_fonts[0] = LoadFont("art/fonts/Author Handwriting.otf");
    sticky_fonts[1] = LoadFont("art/fonts/Milk Cake.otf");
    sticky_fonts[2] = LoadFont("art/fonts/Tomatoes-O8L8.ttf");
    sticky_fonts[3] = LoadFont("art/fonts/Vanilla Pancake.otf");
}

void MinigameManager::unload()
{
    UnloadTexture(journal_text);
    UnloadTexture(pencil_hor);
    UnloadTexture(pencil_ver);
    UnloadTexture(sticker_texts[0]);
    UnloadTexture(sticker_texts[1]);
    UnloadTexture(sticker_texts[2]);

    UnloadFont(sticky_fonts[0]);
    UnloadFont(sticky_fonts[1]);
    UnloadFont(sticky_fonts[2]);
    UnloadFont(sticky_fonts[3]);
}