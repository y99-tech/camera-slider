// Bluetooth LE control using the Nordic UART Service (NUS).
// Works with: the slider web page opened in Chrome (Web Bluetooth),
// "Serial Bluetooth Terminal" (Android), "nRF Connect", "Bluefruit Connect" (iOS/Android).
// Send the same text commands as over USB serial, one per line.
#include "net.h"
#include "config.h"
#include "commands.h"
#include <NimBLEDevice.h>

#define NUS_SERVICE "6E400001-B5A3-F393-E0A9-E50E24DCCA9E"
#define NUS_RX      "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"   // phone -> slider
#define NUS_TX      "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"   // slider -> phone

static NimBLECharacteristic *txChar = nullptr;
static volatile bool connected = false;
static String rxBuf;
static String pending;              // complete lines waiting to be executed
static SemaphoreHandle_t lock = nullptr;   // BLE callbacks run in another task

class ServerCb : public NimBLEServerCallbacks {
    void onConnect(NimBLEServer *s) override { connected = true; }
    void onDisconnect(NimBLEServer *s) override {
        connected = false;
        NimBLEDevice::startAdvertising();
    }
};

class RxCb : public NimBLECharacteristicCallbacks {
    void onWrite(NimBLECharacteristic *c) override {
        std::string v = c->getValue();
        xSemaphoreTake(lock, portMAX_DELAY);
        for (char ch : v) {
            if (ch == '\n' || ch == '\r' || ch == ';') {
                if (rxBuf.length()) { pending += rxBuf; pending += '\n'; rxBuf = ""; }
            } else rxBuf += ch;
        }
        xSemaphoreGive(lock);
    }
};

static void bleSend(const String &s) {
    if (!connected || !txChar) return;
    String out = s;
    out.replace("\n", "\r");      // one reply = one line for the web page
    out += "\n";
    // chunk to stay below the smallest common MTU
    const size_t chunk = 180;
    for (size_t i = 0; i < out.length(); i += chunk) {
        String part = out.substring(i, min(out.length(), i + chunk));
        txChar->setValue((uint8_t *)part.c_str(), part.length());
        txChar->notify();
        delay(5);
    }
}

void bleInit() {
    lock = xSemaphoreCreateMutex();
    NimBLEDevice::init(BLE_NAME);
    NimBLEDevice::setMTU(247);
    NimBLEDevice::setPower(ESP_PWR_LVL_P9);
    NimBLEServer *srv = NimBLEDevice::createServer();
    srv->setCallbacks(new ServerCb());
    NimBLEService *svc = srv->createService(NUS_SERVICE);
    txChar = svc->createCharacteristic(NUS_TX, NIMBLE_PROPERTY::NOTIFY);
    NimBLECharacteristic *rx = svc->createCharacteristic(NUS_RX, NIMBLE_PROPERTY::WRITE | NIMBLE_PROPERTY::WRITE_NR);
    rx->setCallbacks(new RxCb());
    svc->start();
    NimBLEAdvertising *adv = NimBLEDevice::getAdvertising();
    adv->addServiceUUID(NUS_SERVICE);
    adv->setScanResponse(true);
    adv->start();
    Serial.println("Bluetooth LE: advertising as \"" BLE_NAME "\"");
}

void bleLoop() {
    if (!pending.length()) return;
    String work;
    xSemaphoreTake(lock, portMAX_DELAY);
    work = pending; pending = "";
    xSemaphoreGive(lock);
    int start = 0;
    while (start < (int)work.length()) {
        int nl = work.indexOf('\n', start);
        if (nl < 0) nl = work.length();
        String line = work.substring(start, nl);
        start = nl + 1;
        String r = runCommand(line);
        if (r.length()) bleSend(r);
    }
}

bool bleConnected() { return connected; }
