# Firmware (ESP32)

ESP32 Arduino firmware built on [FastAccelStepper](https://github.com/gin66/FastAccelStepper) for smooth
step generation and [NimBLE-Arduino](https://github.com/h2zero/NimBLE-Arduino) for Bluetooth.

* `ui/index.html` is the control page. It's built into the firmware at compile time (`scripts/embed_ui.py`
  → `src/web_ui.h`). Edit the HTML, not the header.
* Everything goes through one text command parser (`src/commands.cpp`), whether it comes from Wi-Fi,
  Bluetooth, USB or the buttons.

## Flash with PlatformIO (recommended)

1. Install [VS Code](https://code.visualstudio.com) + the **PlatformIO** extension (or `pip install platformio`).
2. Open the `firmware` folder.
3. Plug in the ESP32 by USB (switch the slider's 12 V **off**). Then:
   ```
   pio run -t upload
   pio device monitor
   ```
   Some boards need the **BOOT** button held while "Connecting…" shows.
4. You should see:
   ```
   === DIY Camera Slider ===
   Wi-Fi AP "CamSlider-1A2B" password "slider123"  ->  http://192.168.4.1
   Bluetooth LE: advertising as "CamSlider"
   ```

USB driver: most DevKit V1 boards use **CP2102** or **CH340**. Install that driver on Windows if no COM port shows up.

## Flash with Arduino IDE

1. Boards Manager → install **esp32 by Espressif**, version **2.0.17**.
2. Library Manager → install **FastAccelStepper** (0.31.x) and **NimBLE-Arduino** (**1.4.x**, not 2.x).
3. Make a folder `CameraSlider`, copy all files from `src/` into it, add an empty `CameraSlider.ino`.
   Run `python scripts/embed_ui.py` first if you changed the HTML.
4. Board **ESP32 Dev Module**, Partition Scheme **Huge APP (3MB No OTA)**. Upload.

## Using it

### Wi-Fi (phone or laptop)
* Join **CamSlider-XXXX** (password `slider123`) → open **http://192.168.4.1**.
* Or set your home/studio Wi-Fi in *Settings* (name + password → Save → Reboot). Then open
  **http://slider.local** (or the IP printed on the serial monitor).

### Bluetooth
* **Android / desktop Chrome:** copy `ui/index.html` to the phone and open it in Chrome (or host it on
  GitHub Pages), tap **Bluetooth**, pick *CamSlider*. Same page, no Wi-Fi needed.
* **iPhone:** Safari has no Web Bluetooth. Use the free **Bluefruit Connect** or **nRF Connect** app →
  UART, and type the commands (e.g. `RUN 30 40 1`). Or use the Wi-Fi page.
* **Android terminal:** "Serial Bluetooth Terminal" app → Devices → Bluetooth LE → CamSlider.

### Commands
See [../docs/COMMANDS.md](../docs/COMMANDS.md).

## Tested

Compiled against Arduino-ESP32 2.0.17, FastAccelStepper 0.31.2 and NimBLE-Arduino 1.4.2
(sketch ≈ 1.1 MB, 35 % of the Huge APP partition). It hasn't run on real hardware yet, so check
direction and endstops with the camera off the first time.
