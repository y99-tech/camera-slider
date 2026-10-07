// ============================================================
//  DIY Camera Slider - ESP32 firmware
//  Control: Wi-Fi web page, Bluetooth LE, USB serial, rotary knob + 2 buttons
// ============================================================
#include <Arduino.h>
#include "config.h"
#include "settings.h"
#include "motion.h"
#include "commands.h"
#include "net.h"

// ---------------- rotary encoder (electronic hand wheel) ----------------
static volatile int32_t encCount = 0;
static volatile uint8_t encState = 0;

static void IRAM_ATTR encIsr() {
    // full quadrature decode; KY-040 gives 4 counts per detent
    static const int8_t table[16] = {0, -1, 1, 0, 1, 0, 0, -1, -1, 0, 0, 1, 0, 1, -1, 0};
    encState = ((encState << 2) | (digitalRead(PIN_ENC_A) << 1) | digitalRead(PIN_ENC_B)) & 0x0F;
    encCount += table[encState];
}

// ---------------- buttons with short / long press ----------------
struct Button {
    uint8_t  pin;
    bool     down = false, longFired = false;
    uint32_t t0 = 0;
    explicit Button(uint8_t p) : pin(p) {}
    // returns 1 = short press (on release), 2 = long press (after 800 ms)
    int poll() {
        bool pressed = digitalRead(pin) == LOW;
        uint32_t now = millis();
        if (pressed && !down) { down = true; longFired = false; t0 = now; }
        else if (pressed && down && !longFired && now - t0 > 800) { longFired = true; return 2; }
        else if (!pressed && down) {
            down = false;
            if (!longFired && now - t0 > 30) return 1;
        }
        return 0;
    }
};
static Button btnRun(PIN_BTN_RUN), btnStop(PIN_BTN_STOP), btnEnc(PIN_ENC_SW);
static bool fineJog = false;

static void reply(const char *src, const String &r) {
    if (r.length()) Serial.printf("[%s] %s\n", src, r.c_str());
}

static void localControls() {
    // knob: jog 5 mm per click (0.5 mm in fine mode)
    static int32_t lastDetent = 0;
    int32_t det = encCount / 4;
    if (det != lastDetent) {
        int32_t d = det - lastDetent;
        lastDetent = det;
        if (motionIsBusy() && motionMode() != Mode::Move) motionStop();
        else reply("knob", runCommand("JOG " + String(d * (fineJog ? 0.5f : 5.0f), 2)));
    }

    switch (btnRun.poll()) {
        case 1: reply("btn", motionIsBusy() ? (motionStop(), String("ok stop")) : runCommand("RUN")); break;
        case 2: reply("btn", runCommand("SETA")); break;
    }
    switch (btnStop.poll()) {
        case 1: motionStop(); reply("btn", "ok stop"); break;
        case 2: reply("btn", runCommand("SETB")); break;
    }
    switch (btnEnc.poll()) {
        case 1: fineJog = !fineJog; reply("knob", fineJog ? "fine jog 0.5 mm" : "coarse jog 5 mm"); break;
        case 2: reply("knob", runCommand(motionMotorOn() ? "MOTOR OFF" : "MOTOR ON")); break;
    }
}

// LED: solid = ready, slow blink = not homed, fast blink = moving, double blink = hand mode
static void statusLed() {
    uint32_t t = millis();
    bool on;
    if (!motionMotorOn())       on = (t % 1500) < 100 || ((t % 1500) > 250 && (t % 1500) < 350);
    else if (motionIsBusy())    on = (t / 100) % 2;
    else if (!motionIsHomed())  on = (t / 600) % 2;
    else                        on = true;
    digitalWrite(PIN_LED, on);
    digitalWrite(PIN_LED_ONBOARD, on);
}

// ------------------------------------------------------------
void setup() {
    Serial.begin(115200);
    delay(200);
    Serial.println("\n=== DIY Camera Slider ===");

    pinMode(PIN_LED, OUTPUT);
    pinMode(PIN_LED_ONBOARD, OUTPUT);
    pinMode(PIN_ENC_A, INPUT_PULLUP);
    pinMode(PIN_ENC_B, INPUT_PULLUP);
    pinMode(PIN_ENC_SW, INPUT_PULLUP);
    pinMode(PIN_BTN_RUN, INPUT_PULLUP);
    pinMode(PIN_BTN_STOP, INPUT_PULLUP);
    attachInterrupt(digitalPinToInterrupt(PIN_ENC_A), encIsr, CHANGE);
    attachInterrupt(digitalPinToInterrupt(PIN_ENC_B), encIsr, CHANGE);

    settingsLoad();
    motionInit();
    netInit();
    bleInit();
    if (cfg.homeOnBoot) reply("boot", motionHome());
    Serial.println("Type HELP for commands.");
}

void loop() {
    static String line;
    while (Serial.available()) {
        char ch = Serial.read();
        if (ch == '\n' || ch == '\r') {
            if (line.length()) { reply("usb", runCommand(line)); line = ""; }
        } else if (line.length() < 200) line += ch;
    }
    motionLoop();
    netLoop();
    bleLoop();
    localControls();
    statusLed();
}
