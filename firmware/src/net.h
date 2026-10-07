#pragma once
#include <Arduino.h>

void   netInit();     // Wi-Fi (station if configured, else own access point) + web server
void   netLoop();
String netInfo();

void   bleInit();     // Bluetooth LE "Nordic UART" service
void   bleLoop();
bool   bleConnected();
