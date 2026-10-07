#pragma once
// ============================================================
//  Pin map and defaults - see docs/WIRING.md
// ============================================================

// --- TMC2209 (STEP/DIR standalone mode, MS1 = MS2 = 3V3 -> 1/16 step) ---
#define PIN_STEP        25
#define PIN_DIR         26
#define PIN_EN          27      // LOW = driver enabled

// --- End stops (micro switch to GND, NO contact) ---
#define PIN_END_MIN     32      // motor end  (position 0)
#define PIN_END_MAX     33      // idler end  (position = travel)

// --- Optional local controls ---
#define PIN_ENC_A       18      // KY-040 CLK
#define PIN_ENC_B       19      // KY-040 DT
#define PIN_ENC_SW      5       // KY-040 SW
#define PIN_BTN_RUN     16      // button 1: run / long = set A
#define PIN_BTN_STOP    17      // button 2: stop / long = set B
#define PIN_LED         4       // status LED (+330R), onboard LED is GPIO2
#define PIN_LED_ONBOARD 2
#define PIN_SHUTTER     23      // PC817 opto -> camera remote (time-lapse)

// --- Mechanics defaults (changeable with SET ... / SAVE) ---
// 200 steps/rev * 16 microsteps / (20 teeth * 2 mm) = 80 steps/mm
#define DEF_STEPS_PER_MM   80.0f
#define DEF_MAX_SPEED      120.0f   // mm/s
#define DEF_ACCEL          250.0f   // mm/s^2
#define DEF_TRAVEL         600.0f   // usable travel for an 800 mm rail (see docs)
#define DEF_HOME_SPEED     25.0f    // mm/s
#define HOME_BACKOFF_MM    3.0f

#define AP_SSID_PREFIX     "CamSlider-"
#define DEF_AP_PASS        "slider123"   // >= 8 chars
#define MDNS_NAME          "slider"      // http://slider.local
#define BLE_NAME           "CamSlider"
