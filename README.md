# DIY Motorized Camera Slider (ESP32 · Wi-Fi + Bluetooth)

A 3D-printed, belt-driven camera slider on two 12 mm steel rods, sized for a
**Sony A7 IV + FE 24-70 mm F2.8 GM II** (≈ 1.4 kg, ≈ 1.8 kg with a ball head).
It's built from parts you can buy in Egyptian electronics/maker shops, and every printed part fits a
**Creality Ender-3 V3**.

![assembly](docs/img/assembly.png)

## Features

* **Mount it 3 ways:** on **2 tripods** (1/4"-20 and 3/8"-16 threads under each end), on **1 tripod**, or flat on a **table/floor** (optional wide feet).
* **Drive it 4 ways:**
  * **Motorized**: NEMA17 + silent TMC2209 driver, smooth eased moves for video
  * **By hand**: motor released, push the carriage
  * **Hand crank**: wheel on the idler end
  * **Fully free**: quick-release belt clamp
  * Also a rotary-knob "electronic hand wheel" on the control box
* **Control:**
  * **Wi-Fi web app** on any phone, no app to install. Uses its own hotspot or joins your Wi-Fi
  * **Bluetooth LE**: the same web page in Chrome, or any BLE-UART app
  * USB serial
  * Optional physical buttons
* **Moves:**
  * A→B with duration and ease, from 1 s to many hours
  * Bounce / loop
  * 6-keyframe sequencer
  * Shoot-move-shoot time-lapse with a camera shutter output
  * Stop-motion step
  * Homing with endstops, soft limits, and settings stored in flash

## Repository

| Folder | What's inside |
|--------|---------------|
| [`cad/`](cad) | Parametric **OpenSCAD** source (`config.scad`, `parts.scad`, `assembly.scad`) and ready-to-print **STLs** in `cad/stl/` |
| [`firmware/`](firmware) | ESP32 firmware (PlatformIO / Arduino IDE) + the web UI (`firmware/ui/index.html`) |
| [`docs/SHOPPING_LIST.md`](docs/SHOPPING_LIST.md) | **Parts to buy**, with search keywords per shop group (+ [CSV](docs/SHOPPING_LIST.csv) to fill in prices) |
| [`docs/PRINTING.md`](docs/PRINTING.md) | Ender-3 V3 print settings, orientation, fit tuning |
| [`docs/WIRING.md`](docs/WIRING.md) | Pin map, power, motor current, endstops, shutter cable |
| [`docs/ASSEMBLY.md`](docs/ASSEMBLY.md) | Step-by-step build, mounting & hand-drive modes |
| [`docs/COMMANDS.md`](docs/COMMANDS.md) | Every command (Wi-Fi / Bluetooth / USB) and setting |

## Key specs

| | |
|---|---|
| Rails | 2 × 12 mm chrome rod, 100 mm apart, 800 mm long (≈ 600 mm travel). 1000 mm works too |
| Bearings | 4 × LM12UU |
| Drive | NEMA17, GT2 6 mm belt, 20T pulleys → 80 steps/mm at 1/16 microstep |
| Speed | up to 120 mm/s (configurable), down to < 0.01 mm/s for long time-lapses |
| Controller | ESP32-WROOM-32 DevKit V1 + TMC2209 + LM2596 |
| Power | 12 V 2 A adapter or 3S 18650 pack |
| Camera mount | 3/8" centre hole + two 1/4" holes on the carriage, for a ball head or fluid head |

## Quick start

1. Buy the parts → [SHOPPING_LIST.md](docs/SHOPPING_LIST.md)
2. Print the STLs → [PRINTING.md](docs/PRINTING.md)
3. Wire it → [WIRING.md](docs/WIRING.md)
4. Assemble → [ASSEMBLY.md](docs/ASSEMBLY.md)
5. Flash the firmware → [firmware/README.md](firmware/README.md)
6. Join Wi-Fi **CamSlider-XXXX** (pw `slider123`), open **http://192.168.4.1**, press **Home**.

## Credits / inspiration

Feature ideas (A-B with ease, bounce, keyframe sequencer, time-lapse, stop motion) were inspired by the
excellent open-source [DigitalBird Camera Slider](https://github.com/digitalbird01/DigitalBird-Camera-Slider)
project ([Thingiverse](https://www.thingiverse.com/colinh3d/designs) ·
[YouTube](https://www.youtube.com/channel/UC7_bqm5Ea4Gn_P09HKYqyQA)). This repository is an independent
design. No DigitalBird files or code are copied here. If you want a pan/tilt head or a more complete
system, check out their designs and PCBs.
