#pragma once
#include <Arduino.h>

#define MAX_KEYS 6

struct Keyframe {
    float pos;      // mm
    float secs;     // time to travel from the previous key to this one
};

struct Settings {
    float stepsPerMm;
    float maxSpeed;     // mm/s
    float accel;        // mm/s^2
    float travel;       // mm
    float homeSpeed;    // mm/s
    bool  invertDir;
    bool  useEndstops;
    bool  homeOnBoot;
    // A-B move
    float posA, posB;
    float runSecs;
    float easePct;      // % of the move time spent ramping (0..100)
    // stop motion step
    float smStep;
    // keyframe sequencer
    uint8_t  nKeys;
    Keyframe keys[MAX_KEYS];
    // network
    String ssid, pass, apPass;
};

extern Settings cfg;

void settingsLoad();
void settingsSave();
void settingsReset();
