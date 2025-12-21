#pragma once

#include "stdlib.h"

struct Note
{
    bool in_right_pos;
    int zone;
    const char* text;
    Vector2 position;
    Vector2 size;

    Color color;
};

static float RandomRange(int min, int max)
{
    return min + (rand() % (max - min));
}

static Vector2 RandomVector(int minW, int maxW, int minH, int maxH)
{
    return Vector2{
        RandomRange(minW, maxW),
            RandomRange(minH, maxH)
    };
}

static Color colors[] =
{
    Color {0xe3,0xd1,0x96,0xff},
    Color {0xbf,0xb4,0x69,0xff},
    Color {219, 188, 140, 0xff},
    Color {195, 196, 110,0xff},
    Color {207, 164, 145,0xff},
    Color {143, 180, 196,0xff},
    Color {140, 140, 219,0xff},
    Color {141, 201, 155,0xff}
};

static Color NewColor(int index)
{
    return colors[index % 8];
}

static const char* texts[] =
{
    "King Henry had 6 wives! Just like how you'll have 1 one day!",
    "Remember to carry the one...",
    "You're too cute to forget the process of mitosis!",
    "Ever notice how the windows always look the same outside?"
};