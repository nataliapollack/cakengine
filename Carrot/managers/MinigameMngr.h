#pragma once
#include "raylib.h"

#include "Events.hpp"


#include "notes.h"

class MinigameManager
{
public:
    void init();

    void update();
    void draw();
    void load();
    void unload();

    void PlaceNotes();
    bool GetMinigameStatus();

    void AddNote(int i, Vector2 pos, Vector2 size, Color col, const char* txt, int zone);
    void DrawNote(int i);
    void DrawNoteShadow(int i);
    void DrawNoteOffset(int i);

    void MoveNote2Top(int index);

    void CheckNotesStatus();

    void TriggerMinigame(Event& event);
    void TriggerMinigameEnd(Event& event);
private:
    bool in_minigame;
    bool journal_open;

    bool is_dragging;

    int note_text_it;

    Vector2 offset;

    Rectangle zones[4];
    Note notes[4];
    Vector2 sticker_placement[3];

    Texture journal_text;
    Texture pencil_hor;
    Texture pencil_ver;
    Texture sticker_texts[3];
    Font sticky_fonts[4];
};