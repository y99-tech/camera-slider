#include "commands.h"
#include "motion.h"
#include "settings.h"
#include "net.h"

static const char HELP[] =
    "STATUS | HOME | ZERO | STOP | ESTOP\n"
    "GOTO <mm> [mm/s] | JOG <mm> | MOVE <mm> <sec> [ease%]\n"
    "SETA [mm] | SETB [mm] | GOA | GOB\n"
    "RUN [sec] [ease%] [legs]   legs: 1=A->B 2=A->B->A 0=bounce forever\n"
    "KEY ADD [mm] [sec] | KEY CLR | KEY LIST | SEQ [loops]\n"
    "TL <shots> <interval s> [settle ms] [shutter ms]\n"
    "SNAP [-] | SHUTTER [ms] | MOTOR ON|OFF\n"
    "SET <spm|vmax|accel|travel|hspeed|invert|endstops|homeboot|ease|secs|smstep|ssid|pass|appass> <value>\n"
    "SAVE | CONFIG | RESET | WIFI | REBOOT";

// split into up to 6 tokens
static int tokenize(const String &line, String tok[], int maxTok) {
    int n = 0, i = 0, len = line.length();
    while (i < len && n < maxTok) {
        while (i < len && isspace((unsigned char)line[i])) i++;
        if (i >= len) break;
        int j = i;
        while (j < len && !isspace((unsigned char)line[j])) j++;
        tok[n++] = line.substring(i, j);
        tok[n - 1].trim();
        i = j;
    }
    return n;
}

static bool parseBool(const String &v) {
    String s = v; s.toLowerCase();
    return s == "1" || s == "on" || s == "true" || s == "yes";
}

static String configJson() {
    String s = "{\"spm\":" + String(cfg.stepsPerMm, 3);
    s += ",\"vmax\":" + String(cfg.maxSpeed, 1);
    s += ",\"accel\":" + String(cfg.accel, 1);
    s += ",\"travel\":" + String(cfg.travel, 1);
    s += ",\"hspeed\":" + String(cfg.homeSpeed, 1);
    s += ",\"invert\":" + String(cfg.invertDir ? 1 : 0);
    s += ",\"endstops\":" + String(cfg.useEndstops ? 1 : 0);
    s += ",\"homeboot\":" + String(cfg.homeOnBoot ? 1 : 0);
    s += ",\"smstep\":" + String(cfg.smStep, 2);
    s += ",\"ssid\":\"" + cfg.ssid + "\"}";
    return s;
}

String runCommand(const String &raw) {
    String line = raw; line.trim();
    if (!line.length()) return "";
    String t[6];
    int n = tokenize(line, t, 6);
    String c = t[0]; c.toUpperCase();
    auto f = [&](int i, float def) { return n > i ? t[i].toFloat() : def; };

    if (c == "HELP" || c == "?")  return HELP;
    if (c == "STATUS" || c == "S") return motionStatusJson();
    if (c == "CONFIG")            return configJson();
    if (c == "HOME")              return motionHome();
    if (c == "ZERO")              return motionZero();
    if (c == "STOP")              { motionStop();  return "ok stop"; }
    if (c == "ESTOP" || c == "!") { motionEStop(); return "ok estop"; }
    if (c == "GOTO" && n >= 2)    return motionGoto(t[1].toFloat(), f(2, 0));
    if (c == "JOG"  && n >= 2)    return motionJog(t[1].toFloat());
    if (c == "MOVE" && n >= 3)    return motionTimedMove(t[1].toFloat(), t[2].toFloat(), f(3, cfg.easePct));

    if (c == "SETA") { cfg.posA = n > 1 ? t[1].toFloat() : motionPos(); settingsSave(); return "ok A=" + String(cfg.posA, 1); }
    if (c == "SETB") { cfg.posB = n > 1 ? t[1].toFloat() : motionPos(); settingsSave(); return "ok B=" + String(cfg.posB, 1); }
    if (c == "GOA")  return motionGoto(cfg.posA);
    if (c == "GOB")  return motionGoto(cfg.posB);
    if (c == "RUN") {
        if (n > 1) cfg.runSecs = t[1].toFloat();
        if (n > 2) cfg.easePct = constrain(t[2].toFloat(), 0.0f, 100.0f);
        int legs = n > 3 ? t[3].toInt() : 1;
        return motionRunAB(cfg.runSecs, cfg.easePct, legs);
    }

    if (c == "KEY" && n >= 2) {
        String sub = t[1]; sub.toUpperCase();
        if (sub == "CLR") { cfg.nKeys = 0; settingsSave(); return "ok keys cleared"; }
        if (sub == "LIST") {
            String s = "keys:";
            for (int i = 0; i < cfg.nKeys; i++)
                s += " [" + String(i + 1) + "] " + String(cfg.keys[i].pos, 1) + "mm/" + String(cfg.keys[i].secs, 1) + "s";
            return s;
        }
        if (sub == "ADD") {
            if (cfg.nKeys >= MAX_KEYS) return "err max " + String(MAX_KEYS) + " keys";
            Keyframe &k = cfg.keys[cfg.nKeys++];
            k.pos  = n > 2 ? t[2].toFloat() : motionPos();
            k.secs = n > 3 ? t[3].toFloat() : 10;
            settingsSave();
            return "ok key " + String(cfg.nKeys) + " = " + String(k.pos, 1) + "mm in " + String(k.secs, 1) + "s";
        }
    }
    if (c == "SEQ") return motionSequence(n > 1 ? t[1].toInt() : 1);

    if (c == "TL" && n >= 3)
        return motionTimelapse(t[1].toInt(), t[2].toFloat(), (int)f(3, 500), (int)f(4, 200));
    if (c == "SNAP")    return motionSnap(n > 1 && t[1] == "-" ? -1 : 1);
    if (c == "SHUTTER") { shutterPulse((int)f(1, 200)); return "ok shutter"; }

    if (c == "MOTOR" && n >= 2) {
        bool on = parseBool(t[1]);
        motionSetMotor(on);
        return on ? "ok motor on" : "ok motor off - hand mode";
    }

    if (c == "SET" && n >= 3) {
        String k = t[1]; k.toLowerCase();
        // value = rest of the line (Wi-Fi names/passwords may contain spaces)
        int at = line.indexOf(t[1], 3) + t[1].length();
        String v = line.substring(at); v.trim();
        if      (k == "spm")      cfg.stepsPerMm = v.toFloat();
        else if (k == "vmax")     cfg.maxSpeed   = v.toFloat();
        else if (k == "accel")    cfg.accel      = v.toFloat();
        else if (k == "travel")   cfg.travel     = v.toFloat();
        else if (k == "hspeed")   cfg.homeSpeed  = v.toFloat();
        else if (k == "invert")   cfg.invertDir  = parseBool(v);
        else if (k == "endstops") cfg.useEndstops = parseBool(v);
        else if (k == "homeboot") cfg.homeOnBoot = parseBool(v);
        else if (k == "ease")     cfg.easePct    = constrain(v.toFloat(), 0.0f, 100.0f);
        else if (k == "secs")     cfg.runSecs    = v.toFloat();
        else if (k == "smstep")   cfg.smStep     = v.toFloat();
        else if (k == "ssid")     cfg.ssid       = v == "-" ? "" : v;
        else if (k == "pass")     cfg.pass       = v == "-" ? "" : v;
        else if (k == "appass") {
            if (v.length() < 8) return "err AP password needs 8+ characters";
            cfg.apPass = v;
        }
        else return "err unknown key " + k;
        if (cfg.stepsPerMm < 1) cfg.stepsPerMm = 1;
        if (cfg.maxSpeed < 1)   cfg.maxSpeed = 1;
        if (cfg.accel < 1)      cfg.accel = 1;
        motionApplySettings();
        return "ok " + k + " (send SAVE to keep after reboot)";
    }
    if (c == "SAVE")   { settingsSave(); return "ok saved"; }
    if (c == "RESET")  { settingsReset(); settingsSave(); motionApplySettings(); return "ok factory defaults"; }
    if (c == "WIFI")   return netInfo();
    if (c == "REBOOT") { delay(200); ESP.restart(); return "ok"; }

    return "err unknown command - send HELP";
}
