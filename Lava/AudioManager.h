#pragma once
#include "Events.hpp"

#include "raylib.h"

enum AUDIO
{
    BARR_DOWN,
    BARR_UP,
    HEALTH_DMG,
    CAM_SWAP,
    FAX,
    BUTTON_PRESS,
    ENERGY_SOUND,
    CLICK1,
    CLICK2,
    CLICK3,
    CLICK4,
    CLICK5,
    CLICK6,
    LAZER,
    LIGHT_ON,
    LIGHT_OFF,
    LOW_BATT,
    MINI_SFX,
    NIGHT_START,
    RADIO_SOUND,
    SOLAR_PLACE,
    RADUIO_STOP,
    AUDIO_COUNT
};

class Audio
{
public:
    void TriggerSFX(Event& event);

    void TriggerMusic(Event& event);

    void update();
    void init();
    void load();
    void unload();

private:
    int current_theme;
    Music theme[2];

    Sound sfx[AUDIO_COUNT];
};