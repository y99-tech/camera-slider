#include "motion.h"
#include "config.h"
#include "settings.h"
#include <FastAccelStepper.h>

static FastAccelStepperEngine engine;
static FastAccelStepper *st = nullptr;

static Mode     mode    = Mode::Idle;
static bool     homed   = false;
static bool     motorOn = true;
static int32_t  target  = 0;           // steps
static String   lastMsg = "";

// homing
static uint8_t  homePhase = 0;
// A-B / sequence
static int      legsLeft = 0;          // <0 = forever
static bool     legToB   = true;
static bool     prepos   = false;      // currently driving to the start point
static float    jobSecs = 0, jobEase = 0;
static int      seqIdx = 0, seqDir = 1, seqLoops = 0;
// time-lapse
static int      tlShots = 0, tlShot = 0, tlSettle = 0, tlShutter = 0;
static uint32_t tlInterval = 0, tlNextAt = 0, tlPhaseAt = 0;
static uint8_t  tlPhase = 0;           // 0 move, 1 settle, 2 shoot, 3 wait
// shutter
static uint32_t shutterOffAt = 0;

// ------------------------------------------------------------
static inline int32_t mmToSteps(float mm) { return lroundf(mm * cfg.stepsPerMm); }
static inline float   stepsToMm(int32_t s) { return s / cfg.stepsPerMm; }
static inline bool    endMin() { return cfg.useEndstops && digitalRead(PIN_END_MIN) == LOW; }
static inline bool    endMax() { return cfg.useEndstops && digitalRead(PIN_END_MAX) == LOW; }

static float clampTravel(float mm) {
    if (mm < 0) return 0;
    if (mm > cfg.travel) return cfg.travel;
    return mm;
}

static void setSpeedMm(float v, float a) {
    float hz = v * cfg.stepsPerMm;
    if (hz < 0.07f) hz = 0.07f;          // library limit: ~250 M ticks (15.6 s) per step
    st->setSpeedInMilliHz((uint32_t)(hz * 1000.0f));
    int32_t acc = (int32_t)(a * cfg.stepsPerMm);
    st->setAcceleration(acc < 1 ? 1 : acc);
}

static void startMove(float mm, float v, float a) {
    target = mmToSteps(clampTravel(mm));
    setSpeedMm(v, a);
    st->moveTo(target);
}

// timed move: speed + accel chosen so the move takes `secs` with `easePct` ramps
static float startTimed(float mm, float secs, float easePct) {
    mm = clampTravel(mm);
    float d = fabsf(mm - motionPos());
    if (d < 0.01f) { target = mmToSteps(mm); return 0; }
    if (secs < 0.2f) secs = 0.2f;
    float tr = (easePct / 100.0f) * secs / 2.0f;     // ramp-up time (= ramp-down)
    if (tr < 0.05f) tr = 0.05f;
    if (tr > secs / 2.0f) tr = secs / 2.0f;
    float v = d / (secs - tr);
    float a = v / tr;
    if (v > cfg.maxSpeed) { v = cfg.maxSpeed; a = max(a, cfg.accel); }
    startMove(mm, v, a);
    return v;
}

static bool needHomed(String &err) {
    if (!motorOn) { err = "err motor is off (hand mode) - send MOTOR ON"; return false; }
    if (!homed)   { err = "err not homed - send HOME (or ZERO without endstops)"; return false; }
    return true;
}

static void finishJob(const char *msg) {
    mode = Mode::Idle;
    lastMsg = msg;
}

// ------------------------------------------------------------
void motionInit() {
    pinMode(PIN_END_MIN, INPUT_PULLUP);
    pinMode(PIN_END_MAX, INPUT_PULLUP);
    pinMode(PIN_SHUTTER, OUTPUT);
    digitalWrite(PIN_SHUTTER, LOW);

    engine.init();
    st = engine.stepperConnectToPin(PIN_STEP);
    if (!st) { Serial.println("stepper init failed"); return; }
    st->setDirectionPin(PIN_DIR, !cfg.invertDir);
    st->setEnablePin(PIN_EN, true);       // low active
    st->setAutoEnable(false);
    st->enableOutputs();
    motionApplySettings();
    if (!cfg.useEndstops) homed = true;   // position 0 = where it was switched on
}

void motionApplySettings() {
    if (!st) return;
    st->setDirectionPin(PIN_DIR, !cfg.invertDir);
    setSpeedMm(cfg.maxSpeed, cfg.accel);
}

void shutterPulse(int ms) {
    digitalWrite(PIN_SHUTTER, HIGH);
    shutterOffAt = millis() + (uint32_t)max(ms, 20);
}

// ------------------------------------------------------------
String motionHome() {
    if (!motorOn) return "err motor is off (hand mode)";
    motionStop();
    if (!cfg.useEndstops) { st->setCurrentPosition(0); target = 0; homed = true; return "ok zeroed (endstops disabled)"; }
    homed = false;
    mode = Mode::Homing;
    homePhase = 0;
    lastMsg = "homing";
    return "ok homing";
}

String motionZero() {
    motionStop();
    st->setCurrentPosition(0);
    target = 0;
    homed = true;
    return "ok position set to 0";
}

String motionGoto(float mm, float speed) {
    String e; if (!needHomed(e)) return e;
    if (speed <= 0 || speed > cfg.maxSpeed) speed = cfg.maxSpeed;
    mode = Mode::Move;
    startMove(mm, speed, cfg.accel);
    return "ok goto " + String(clampTravel(mm), 1);
}

String motionJog(float d) {
    if (!motorOn) return "err motor is off (hand mode)";
    if (mode != Mode::Idle && mode != Mode::Move) return "err busy";
    if (!st->isRunning()) target = st->getCurrentPosition();   // drop any stale target
    mode = Mode::Move;
    int32_t t = target + mmToSteps(d);
    if (homed) {
        int32_t lo = 0, hi = mmToSteps(cfg.travel);
        t = constrain(t, lo, hi);
    }
    target = t;
    setSpeedMm(min(cfg.maxSpeed, 60.0f), cfg.accel);
    st->moveTo(target);
    return "ok jog";
}

String motionTimedMove(float mm, float secs, float easePct) {
    String e; if (!needHomed(e)) return e;
    mode = Mode::Move;
    float v = startTimed(mm, secs, easePct);
    return "ok move v=" + String(v, 2) + "mm/s";
}

String motionRunAB(float secs, float easePct, int legs) {
    String e; if (!needHomed(e)) return e;
    if (fabsf(cfg.posA - cfg.posB) < 1) return "err A and B are the same";
    jobSecs = secs; jobEase = easePct;
    legsLeft = legs <= 0 ? -1 : legs;
    // start from whichever end we are closer to
    float p = motionPos();
    legToB = fabsf(p - cfg.posA) <= fabsf(p - cfg.posB);
    prepos = true;
    mode = Mode::RunAB;
    startMove(legToB ? cfg.posA : cfg.posB, cfg.maxSpeed, cfg.accel);
    return String("ok run ") + (legToB ? "A->B" : "B->A");
}

String motionSequence(int loops) {
    String e; if (!needHomed(e)) return e;
    if (cfg.nKeys < 2) return "err need at least 2 keyframes (KEY ADD)";
    seqIdx = 0; seqDir = 1;
    seqLoops = loops <= 0 ? -1 : loops;
    prepos = true;
    mode = Mode::Sequence;
    startMove(cfg.keys[0].pos, cfg.maxSpeed, cfg.accel);
    return "ok sequence";
}

static float tlPosFor(int i) {
    float f = tlShots > 1 ? (float)i / (tlShots - 1) : 1.0f;
    float s = f * f * (3 - 2 * f);                       // smoothstep
    float e = constrain(cfg.easePct, 0.0f, 100.0f) / 100.0f;
    f = f * (1 - e) + s * e;
    return cfg.posA + (cfg.posB - cfg.posA) * f;
}

String motionTimelapse(int shots, float intervalS, int settleMs, int shutterMs) {
    String e; if (!needHomed(e)) return e;
    if (shots < 2) return "err shots must be >= 2";
    if (intervalS < 0.5f) return "err interval must be >= 0.5 s";
    tlShots = shots; tlShot = 0;
    tlInterval = (uint32_t)(intervalS * 1000);
    tlSettle = max(settleMs, 0); tlShutter = max(shutterMs, 50);
    tlPhase = 0;
    prepos = true;
    mode = Mode::Timelapse;
    startMove(tlPosFor(0), cfg.maxSpeed, cfg.accel);
    return "ok timelapse " + String(shots) + " shots, " + String(intervalS, 1) + " s, total " +
           String(intervalS * shots / 60.0f, 1) + " min";
}

String motionSnap(int dir) {
    String e; if (!needHomed(e)) return e;
    if (mode != Mode::Idle) return "err busy";
    float sgn = (cfg.posB >= cfg.posA ? 1 : -1) * (dir < 0 ? -1 : 1);
    mode = Mode::Timelapse;           // reuse: move -> settle -> shoot, single shot
    tlShots = 1; tlShot = 0; tlSettle = 300; tlShutter = 150; tlInterval = 0;
    tlPhase = 0; prepos = false;
    startMove(motionPos() + sgn * cfg.smStep, min(cfg.maxSpeed, 30.0f), cfg.accel);
    return "ok snap";
}

void motionStop() {
    if (!st) return;
    st->stopMove();
    if (mode == Mode::Homing) homed = false;
    mode = Mode::Idle;
    lastMsg = "stopped";
}

void motionEStop() {
    if (!st) return;
    st->forceStop();
    if (mode == Mode::Homing) homed = false;
    mode = Mode::Idle;
    lastMsg = "emergency stop";
}

void motionSetMotor(bool on) {
    if (on == motorOn) return;
    if (!on) {
        st->forceStop();
        mode = Mode::Idle;
        st->disableOutputs();
        motorOn = false;
        if (cfg.useEndstops) homed = false;   // carriage will be moved by hand
        lastMsg = "hand mode";
    } else {
        st->enableOutputs();
        motorOn = true;
        target = st->getCurrentPosition();
        lastMsg = homed ? "motor on" : "motor on - HOME needed";
    }
}

// ------------------------------------------------------------
void motionLoop() {
    if (!st) return;
    uint32_t now = millis();

    if (shutterOffAt && (int32_t)(now - shutterOffAt) >= 0) {
        digitalWrite(PIN_SHUTTER, LOW);
        shutterOffAt = 0;
    }

    bool running = st->isRunning();
    int32_t cur = st->getCurrentPosition();

    // ---- end stop protection (except while homing, handled below) ----
    if (mode != Mode::Homing && running) {
        bool goingDown = target < cur;
        if ((goingDown && endMin()) || (!goingDown && endMax())) {
            st->forceStop();
            target = cur;
            mode = Mode::Idle;
            lastMsg = goingDown ? "hit MIN endstop" : "hit MAX endstop";
            return;
        }
    }

    switch (mode) {
    case Mode::Idle:
        break;

    case Mode::Homing:
        // safety: never run further than the whole rail looking for the switch
        if ((homePhase == 2 || homePhase == 4) && cur < -mmToSteps(cfg.travel + 100)) {
            st->forceStop();
            finishJob("homing failed - check MIN endstop wiring");
            break;
        }
        switch (homePhase) {
        case 0:   // if already on the switch, move off it first
            setSpeedMm(cfg.homeSpeed, cfg.accel);
            if (endMin()) { st->setCurrentPosition(0); st->moveTo(mmToSteps(8)); homePhase = 1; }
            else          { st->setCurrentPosition(0); st->runBackward(); homePhase = 2; }
            break;
        case 1:
            if (!st->isRunning()) { st->runBackward(); homePhase = 2; }
            break;
        case 2:   // fast approach
            if (endMin()) {
                st->forceStopAndNewPosition(0);
                setSpeedMm(cfg.homeSpeed, cfg.accel);
                st->moveTo(mmToSteps(HOME_BACKOFF_MM));
                homePhase = 3;
            }
            break;
        case 3:
            if (!st->isRunning()) {
                setSpeedMm(cfg.homeSpeed / 5, cfg.accel);   // slow second touch
                st->runBackward();
                homePhase = 4;
            }
            break;
        case 4:
            if (endMin()) {
                st->forceStopAndNewPosition(0);
                st->moveTo(mmToSteps(HOME_BACKOFF_MM));
                homePhase = 5;
            }
            break;
        case 5:
            if (!st->isRunning()) {
                st->setCurrentPosition(0);
                target = 0;
                homed = true;
                setSpeedMm(cfg.maxSpeed, cfg.accel);
                finishJob("homed");
            }
            break;
        }
        break;

    case Mode::Move:
        if (!running) finishJob("idle");
        break;

    case Mode::RunAB:
        if (running) break;
        if (prepos) {
            prepos = false;
        } else {
            legToB = !legToB;
            if (legsLeft > 0 && --legsLeft == 0) { finishJob("run done"); break; }
        }
        startTimed(legToB ? cfg.posB : cfg.posA, jobSecs, jobEase);
        break;

    case Mode::Sequence:
        if (running) break;
        if (prepos) prepos = false;
        {
            int next = seqIdx + seqDir;
            if (next < 0 || next >= cfg.nKeys) {
                if (seqLoops > 0 && --seqLoops == 0) { finishJob("sequence done"); break; }
                seqDir = -seqDir;
                next = seqIdx + seqDir;
            }
            // segment time belongs to the later key in forward order
            float secs = cfg.keys[seqDir > 0 ? next : seqIdx].secs;
            seqIdx = next;
            startTimed(cfg.keys[seqIdx].pos, secs, cfg.easePct);
        }
        break;

    case Mode::Timelapse:
        switch (tlPhase) {
        case 0:   // moving to shot position
            if (!running) {
                if (prepos) { prepos = false; tlNextAt = now; }
                tlPhase = 1; tlPhaseAt = now;
            }
            break;
        case 1:   // let vibrations settle
            if (now - tlPhaseAt >= (uint32_t)tlSettle) {
                shutterPulse(tlShutter);
                tlPhase = 2; tlPhaseAt = now;
            }
            break;
        case 2:   // exposure / shutter pulse
            if (now - tlPhaseAt >= (uint32_t)tlShutter + 100) {
                tlShot++;
                if (tlShot >= tlShots) { finishJob("timelapse done"); break; }
                tlNextAt += tlInterval;
                tlPhase = 3;
            }
            break;
        case 3:   // wait for next interval, then move
            if ((int32_t)(now - tlNextAt) >= 0) {
                setSpeedMm(min(cfg.maxSpeed, 40.0f), cfg.accel);
                startMove(tlPosFor(tlShot), min(cfg.maxSpeed, 40.0f), cfg.accel);
                tlPhase = 0;
            }
            break;
        }
        break;
    }
}

// ------------------------------------------------------------
float motionPos()      { return st ? stepsToMm(st->getCurrentPosition()) : 0; }
bool  motionIsHomed()  { return homed; }
bool  motionIsBusy()   { return mode != Mode::Idle; }
bool  motionMotorOn()  { return motorOn; }
Mode  motionMode()     { return mode; }

String motionStatusJson() {
    static const char *names[] = {"idle", "homing", "move", "run", "sequence", "timelapse"};
    String s = "{\"pos\":";
    s += String(motionPos(), 2);
    s += ",\"target\":" + String(stepsToMm(target), 2);
    s += ",\"travel\":" + String(cfg.travel, 1);
    s += ",\"homed\":"  + String(homed ? "true" : "false");
    s += ",\"motor\":"  + String(motorOn ? "true" : "false");
    s += ",\"mode\":\"" + String(names[(int)mode]) + "\"";
    s += ",\"a\":" + String(cfg.posA, 1) + ",\"b\":" + String(cfg.posB, 1);
    s += ",\"secs\":" + String(cfg.runSecs, 1) + ",\"ease\":" + String(cfg.easePct, 0);
    s += ",\"keys\":" + String(cfg.nKeys);
    s += ",\"endMin\":" + String(digitalRead(PIN_END_MIN) == LOW ? 1 : 0);
    s += ",\"endMax\":" + String(digitalRead(PIN_END_MAX) == LOW ? 1 : 0);
    if (mode == Mode::Timelapse) s += ",\"shot\":" + String(tlShot) + ",\"shots\":" + String(tlShots);
    s += ",\"msg\":\"" + lastMsg + "\"}";
    return s;
}
