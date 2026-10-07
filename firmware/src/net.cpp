#include "net.h"
#include "config.h"
#include "settings.h"
#include "commands.h"
#include "motion.h"
#include "web_ui.h"
#include <WiFi.h>
#include <WebServer.h>
#include <ESPmDNS.h>

static WebServer server(80);
static bool apMode = false;
static String apName;

static void sendCors() {
    server.sendHeader("Access-Control-Allow-Origin", "*");
    server.sendHeader("Cache-Control", "no-store");
}

static void handleRoot() {
    sendCors();
    server.send_P(200, "text/html", WEB_UI_HTML);
}

static void handleCmd() {
    String c = server.arg("c");
    String r = runCommand(c);
    sendCors();
    server.send(200, "text/plain", r);
}

static void handleStatus() {
    sendCors();
    server.send(200, "application/json", motionStatusJson());
}

void netInit() {
    uint64_t mac = ESP.getEfuseMac();
    char suffix[7];
    snprintf(suffix, sizeof(suffix), "%04X", (uint16_t)(mac >> 32));
    apName = String(AP_SSID_PREFIX) + suffix;

    WiFi.persistent(false);
    WiFi.setSleep(true);      // required when Wi-Fi and Bluetooth run together

    if (cfg.ssid.length()) {
        WiFi.mode(WIFI_STA);
        WiFi.begin(cfg.ssid.c_str(), cfg.pass.c_str());
        Serial.printf("Wi-Fi: joining \"%s\"", cfg.ssid.c_str());
        uint32_t t0 = millis();
        while (WiFi.status() != WL_CONNECTED && millis() - t0 < 12000) { delay(300); Serial.print('.'); }
        Serial.println();
    }
    if (WiFi.status() != WL_CONNECTED) {
        apMode = true;
        WiFi.mode(WIFI_AP);
        WiFi.softAP(apName.c_str(), cfg.apPass.c_str());
    }
    if (MDNS.begin(MDNS_NAME)) MDNS.addService("http", "tcp", 80);

    server.on("/", handleRoot);
    server.on("/cmd", handleCmd);
    server.on("/status", handleStatus);
    server.onNotFound([] { sendCors(); server.send(404, "text/plain", "not found"); });
    server.begin();
    Serial.println(netInfo());
}

void netLoop() { server.handleClient(); }

String netInfo() {
    if (apMode)
        return "Wi-Fi AP \"" + apName + "\" password \"" + cfg.apPass + "\"  ->  http://" +
               WiFi.softAPIP().toString() + "  (or http://" MDNS_NAME ".local)";
    return "Wi-Fi connected to \"" + cfg.ssid + "\"  ->  http://" + WiFi.localIP().toString() +
           "  (or http://" MDNS_NAME ".local)";
}
