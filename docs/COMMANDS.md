# Commands

The same text commands work over **USB serial** (115200 baud), **Bluetooth LE** (Nordic UART service,
device name `CamSlider`) and **Wi-Fi** (`http://<slider>/cmd?c=<command>`). Not case-sensitive.
Positions are in **mm from the motor end** (after homing).

| Command | What it does |
|---------|--------------|
| `HELP` | list commands |
| `STATUS` | JSON: position, mode, A/B, homed, endstops … |
| `HOME` | find the MIN endstop (motor end), back off 3 mm, set 0 |
| `ZERO` | "here is 0" (use when endstops are disabled) |
| `STOP` / `ESTOP` | smooth stop / immediate stop, cancels any job |
| `GOTO <mm> [mm/s]` | go to a position at max (or given) speed |
| `JOG <mm>` | relative move (+ = toward idler end) |
| `MOVE <mm> <sec> [ease%]` | timed move to a position |
| `SETA [mm]`, `SETB [mm]` | store A / B (current position if no value) |
| `GOA`, `GOB` | go to A / B |
| `RUN [sec] [ease%] [legs]` | cinematic A↔B move. legs: `1` one way, `2` there and back, `0` bounce forever. Starts from the nearer end |
| `KEY ADD [mm] [sec]` | add keyframe (max 6). `sec` = time to reach this key from the previous one |
| `KEY LIST`, `KEY CLR` | list / delete keyframes |
| `SEQ [loops]` | play keyframes (ping-pong), `0` = forever |
| `TL <shots> <interval s> [settle ms] [shutter ms]` | shoot-move-shoot time-lapse from A to B |
| `SNAP [-]` | stop-motion: move one step (`smstep`) toward B (or back with `-`) and fire the shutter |
| `SHUTTER [ms]` | test the shutter output |
| `MOTOR ON` / `MOTOR OFF` | motor holding / hand mode |
| `SET <key> <value>` | change a setting (see below) |
| `SAVE` | store settings in flash |
| `CONFIG` | settings as JSON |
| `RESET` | factory defaults |
| `WIFI` | show the IP / hotspot name |
| `REBOOT` | restart |

### Settings (`SET`)

| Key | Default | Meaning |
|-----|---------|---------|
| `spm` | 80 | steps per mm (200 × 16 / (20 teeth × 2 mm)) |
| `vmax` | 120 | max speed mm/s |
| `accel` | 250 | acceleration mm/s² |
| `travel` | 600 | usable travel mm |
| `hspeed` | 25 | homing speed mm/s |
| `invert` | 0 | reverse motor direction |
| `endstops` | 1 | 0 = no switches fitted |
| `homeboot` | 0 | home automatically at power-on |
| `ease` | 30 | % of the move spent speeding up and slowing down. 0 = linear, 100 = all ramp |
| `secs` | 20 | default RUN duration |
| `smstep` | 2 | stop-motion step mm |
| `ssid` / `pass` | – | join your Wi-Fi instead of making a hotspot (`SET ssid -` clears it). Needs `SAVE` + `REBOOT` |
| `appass` | slider123 | hotspot password (8+ chars) |

### Local buttons / knob (optional hardware)

| Control | Short press | Long press (0.8 s) |
|---------|-------------|--------------------|
| Button 1 | RUN (or stop if moving) | Set A here |
| Button 2 | STOP | Set B here |
| Knob turn | jog 5 mm per click (0.5 mm fine) | – |
| Knob press | toggle fine / coarse | motor on / off (hand mode) |

### Status LED

solid = ready · slow blink = not homed · fast blink = moving · double blink = hand mode
