#pragma once
#include <Arduino.h>

// Executes one text command (from Serial, Bluetooth, Wi-Fi or local buttons)
// and returns the reply text. See docs/COMMANDS.md.
String runCommand(const String &line);
