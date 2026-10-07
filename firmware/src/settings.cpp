#include "settings.h"
#include "config.h"
#include <Preferences.h>

Settings cfg;
static Preferences prefs;

static void defaults() {
    cfg.stepsPerMm  = DEF_STEPS_PER_MM;
    cfg.maxSpeed    = DEF_MAX_SPEED;
    cfg.accel       = DEF_ACCEL;
    cfg.travel      = DEF_TRAVEL;
    cfg.homeSpeed   = DEF_HOME_SPEED;
    cfg.invertDir   = false;
    cfg.useEndstops = true;
    cfg.homeOnBoot  = false;
    cfg.posA        = 0;
    cfg.posB        = DEF_TRAVEL;
    cfg.runSecs     = 20;
    cfg.easePct     = 30;
    cfg.smStep      = 2;
    cfg.nKeys       = 0;
    cfg.ssid = ""; cfg.pass = ""; cfg.apPass = DEF_AP_PASS;
}

void settingsLoad() {
    defaults();
    prefs.begin("slider", true);
    cfg.stepsPerMm  = prefs.getFloat("spm",    cfg.stepsPerMm);
    cfg.maxSpeed    = prefs.getFloat("vmax",   cfg.maxSpeed);
    cfg.accel       = prefs.getFloat("accel",  cfg.accel);
    cfg.travel      = prefs.getFloat("travel", cfg.travel);
    cfg.homeSpeed   = prefs.getFloat("hspeed", cfg.homeSpeed);
    cfg.invertDir   = prefs.getBool ("invert", cfg.invertDir);
    cfg.useEndstops = prefs.getBool ("endst",  cfg.useEndstops);
    cfg.homeOnBoot  = prefs.getBool ("hboot",  cfg.homeOnBoot);
    cfg.posA        = prefs.getFloat("a",      cfg.posA);
    cfg.posB        = prefs.getFloat("b",      cfg.posB);
    cfg.runSecs     = prefs.getFloat("rsecs",  cfg.runSecs);
    cfg.easePct     = prefs.getFloat("ease",   cfg.easePct);
    cfg.smStep      = prefs.getFloat("smstep", cfg.smStep);
    cfg.nKeys       = prefs.getUChar("nkeys",  0);
    if (cfg.nKeys > MAX_KEYS) cfg.nKeys = 0;
    if (cfg.nKeys) prefs.getBytes("keys", cfg.keys, sizeof(Keyframe) * cfg.nKeys);
    cfg.ssid   = prefs.getString("ssid",   cfg.ssid);
    cfg.pass   = prefs.getString("pass",   cfg.pass);
    cfg.apPass = prefs.getString("appass", cfg.apPass);
    prefs.end();
}

void settingsSave() {
    prefs.begin("slider", false);
    prefs.putFloat("spm",    cfg.stepsPerMm);
    prefs.putFloat("vmax",   cfg.maxSpeed);
    prefs.putFloat("accel",  cfg.accel);
    prefs.putFloat("travel", cfg.travel);
    prefs.putFloat("hspeed", cfg.homeSpeed);
    prefs.putBool ("invert", cfg.invertDir);
    prefs.putBool ("endst",  cfg.useEndstops);
    prefs.putBool ("hboot",  cfg.homeOnBoot);
    prefs.putFloat("a",      cfg.posA);
    prefs.putFloat("b",      cfg.posB);
    prefs.putFloat("rsecs",  cfg.runSecs);
    prefs.putFloat("ease",   cfg.easePct);
    prefs.putFloat("smstep", cfg.smStep);
    prefs.putUChar("nkeys",  cfg.nKeys);
    prefs.putBytes("keys",   cfg.keys, sizeof(Keyframe) * MAX_KEYS);
    prefs.putString("ssid",   cfg.ssid);
    prefs.putString("pass",   cfg.pass);
    prefs.putString("appass", cfg.apPass);
    prefs.end();
}

void settingsReset() {
    prefs.begin("slider", false);
    prefs.clear();
    prefs.end();
    defaults();
}
