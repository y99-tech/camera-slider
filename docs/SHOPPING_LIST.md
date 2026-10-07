# Shopping / Search List

This is the list to search on the Egyptian shops you gave. I couldn't reach the
shops myself, so the **"Try first"** column is only a guess at which kind of
shop is most likely to stock each item. Write the price and link you find in the
last two columns (or in `SHOPPING_LIST.csv`).

Sized for: **Sony A7 IV (≈660 g) + FE 24-70 GM II (≈695 g) + ball head (≈400 g) ≈ 1.8 kg**,
with an **800 mm** rail by default (1000 mm also works, see notes).

Shop groups used below:

* **ELEC** – ram-e-shop.com, makerselectronics.com, store.fut-electronics.com, free-electronic.com,
  uge-one.com, electrolik-eg.com, circuits-elec.com, microohm-eg.com, ampere-eg.com, sigmastore.net,
  3duino.com, mokwn.com, shop.electech.com.eg, shop.hak.com.eg, lampatronics.com, emtech-eg.com,
  metanoia-tech.com, norisolutions.com, ekostra.com, justpiece.com
* **MECH** – cncegy.com, mechatronics-store.com, alexautomation-eg.com, eisac-automation.com,
  ram-e-shop.com, makerselectronics.com, store.fut-electronics.com
* **3DP** – 3dsmart-shop.com, in3d-shop.com, lancer3d.com, hawk3dvision.com, 3duino.com

---

## A. Motion parts (most important – buy these first)

| # | Item | Spec / what to check | Qty | Search keywords | Try first | Price | Link |
|---|------|----------------------|-----|-----------------|-----------|-------|------|
| A1 | Smooth rod 12 mm | chrome hardened, **800 mm** (or 1000 mm), straight | 2 | `12mm linear rod`, `12mm smooth rod 800mm`, `chrome shaft 12mm`, `optical axis 12mm` | MECH | | |
| A2 | Linear bearing LM12UU | standard (not "L" long, not "OP" open) | 4 | `LM12UU`, `linear bearing 12mm` | MECH, ELEC | | |
| A3 | Stepper motor NEMA17 | 42×42 mm, 40–48 mm long, 1.5–2.0 A, 5 mm shaft (e.g. 17HS4401, 17HS8401) | 1 | `NEMA17`, `17HS4401`, `stepper motor 42`, `42 stepper` | ELEC, MECH | | |
| A4 | Stepper driver TMC2209 | module (BigTreeTech / FYSETC style). Silent = needed for video | 1 | `TMC2209`, `TMC2209 driver`, `silent stepper driver` | ELEC, 3DP | | |
| A4b | *(fallback)* A4988 or DRV8825 | only if no TMC2209; louder | 1 | `A4988`, `DRV8825` | ELEC | | |
| A5 | GT2 pulley 20 teeth | **5 mm bore**, for **6 mm** belt | 2 | `GT2 pulley 20T 5mm`, `GT2 20 teeth pulley` | ELEC, 3DP, MECH | | |
| A6 | GT2 timing belt | **6 mm** wide, open belt. 2 m for 800 mm rail (2.5 m for 1000 mm) | 2 m | `GT2 belt 6mm`, `GT2 timing belt`, `2GT belt` | ELEC, 3DP, MECH | | |
| A7 | Ball bearing 625ZZ | 5×16×5 mm (hand-crank shaft) | 2 | `625ZZ`, `625 bearing`, `bearing 5x16x5` | MECH, 3DP | | |
| A8 | Shaft 5 mm | smooth rod, cut to ~70 mm (or use an M5×70 bolt) | 1 | `5mm linear rod`, `5mm shaft`, `5mm smooth rod` | MECH | | |
| A9 | Micro limit switch | KW11-3Z (or a 3-D printer "endstop module") | 2 | `KW11 limit switch`, `micro switch lever`, `endstop switch` | ELEC, 3DP | | |

## B. Electronics

| # | Item | Spec / what to check | Qty | Search keywords | Try first | Price | Link |
|---|------|----------------------|-----|-----------------|-----------|-------|------|
| B1 | ESP32 dev board | **ESP32-WROOM-32 DevKit V1 (30 or 38 pin)** – has Wi-Fi **and** Bluetooth. Not ESP32-S2 (no BT), not ESP8266 | 1 | `ESP32 devkit`, `ESP32 WROOM 32`, `NodeMCU ESP32` | ELEC | | |
| B2 | Buck converter 12 V → 5 V | LM2596 module (adjustable) or MP1584 / Mini-360 | 1 | `LM2596`, `step down converter`, `buck converter` | ELEC | | |
| B3 | Power adapter 12 V | 12 V **2 A** (or 3 A), 5.5×2.1 mm plug | 1 | `12V 2A adapter`, `12V power supply 5.5` | ELEC | | |
| B4 | DC jack, panel mount | 5.5×2.1 mm female, 8 mm hole | 1 | `DC jack panel 5.5 2.1`, `DC005` (PCB) or `DC-022` (panel) | ELEC | | |
| B5 | Rocker switch | KCD1 small (21×15 mm, 19×13 cut-out) | 1 | `KCD1 rocker switch`, `rocker switch 2 pin` | ELEC | | |
| B6 | Electrolytic capacitor | 100 µF 35 V (across driver motor supply) | 1 | `100uf 35v capacitor` | ELEC | | |
| B7 | Perfboard | 7×5 cm (double-sided is nicer) | 1 | `perfboard 7x5`, `PCB prototype board 5x7` | ELEC | | |
| B8 | Female pin headers | 1×40 pin, 2.54 mm (to socket ESP32 + driver) | 2 | `female header 40 pin` | ELEC | | |
| B9 | Connectors | JST-XH 2.54 kit **or** Dupont kit; screw terminal 2-pin ×2 | 1 kit | `JST XH connector kit`, `dupont connector kit`, `terminal block 2 pin` | ELEC | | |
| B10 | Motor cable | 4-core, ~1 m (or the cable that comes with the motor) | 1 | `4 core cable`, `stepper motor cable` | ELEC | | |
| B11 | Hook-up wire | 22 AWG silicone or solid, few colours | 1 set | `22AWG wire`, `jumper wire` | ELEC | | |

### B-optional (nice to have)

| # | Item | Spec | Qty | Search keywords | Try first | Price | Link |
|---|------|------|-----|-----------------|-----------|-------|------|
| B12 | Rotary encoder | KY-040 module (or EC11 + knob). "Electronic hand wheel" + menu | 1 | `KY-040`, `rotary encoder module` | ELEC | | |
| B13 | Push buttons | 12 mm panel / tactile with cap | 2 | `push button 12mm`, `tactile switch 12x12` | ELEC | | |
| B14 | LED 5 mm + 330 Ω resistor | status | 1 | `LED 5mm`, `resistor 330 ohm` | ELEC | | |
| B15 | Optocoupler PC817 + 1 kΩ resistor | camera shutter output (time-lapse) | 1 | `PC817` | ELEC | | |
| B16 | 3S battery: 3× 18650 cells + 3S BMS + 3S holder | cordless use (≈11.1 V) | 1 set | `18650 battery`, `3S BMS 20A`, `18650 holder 3` | ELEC | | |
| B16b | *(alt.)* 12 V power bank / 3S Li-ion pack with DC out | cordless use | 1 | `12V lithium battery pack`, `12V power bank DC` | ELEC | | |
| B17 | Heat-shrink tube | assorted | 1 | `heat shrink` | ELEC | | |

## C. Screws, nuts, small parts

| # | Item | Qty | Used for | Search keywords | Try first | Price | Link |
|---|------|-----|----------|-----------------|-----------|-------|------|
| C1 | M3×10 socket screw | 4 | NEMA17 to motor plate | `M3 screw 10mm`, `M3 allen bolt` | ELEC, 3DP, MECH | | |
| C2 | M3×16 socket screw | 4 | belt clamps | `M3x16` | ″ | | |
| C3 | M3×20 screw | 2 | endstop clips | `M3x20` | ″ | | |
| C4 | M3×8 screw | 4 | box lid | `M3x8` | ″ | | |
| C5 | M3 hex nut | 10 | belt clamps, clips, wheel | `M3 nut` | ″ | | |
| C6 | M3 grub (set) screw ×4–6 mm | 1 | hand wheel on shaft | `M3 grub screw`, `M3 set screw` | ″ | | |
| C7 | M4×16 screw | 4 | rod set-screws in end blocks | `M4x16` | ″ | | |
| C8 | M4×12 screw | 2 | electronics box to end block | `M4x12` | ″ | | |
| C9 | M4 hex nut | 6 | | `M4 nut` | ″ | | |
| C10 | M2×10 screw + nut | 4 | micro switches | `M2 screw` | ″ | | |
| C11 | M5×30 bolt + nyloc nut | 1 | crank handle on hand wheel | `M5x30`, `M5 nylock nut` | ″ | | |
| C12 | M5 washers | 4 | shims on idler shaft | `M5 washer` | ″ | | |
| C13 | **1/4"-20 hex nut** | 4 | tripod threads in end blocks | `1/4 inch nut`, `1/4-20 nut`, `UNC 1/4 nut`, `camera screw nut` | MECH, hardware store | | |
| C14 | **3/8"-16 hex nut** | 2 | tripod threads in end blocks | `3/8 inch nut`, `3/8-16 nut`, `UNC 3/8 nut` | MECH, hardware store | | |
| C15 | 3/8"-16 × 5/8" screw (or 1/4"-20 × 5/8") | 1 | ball head to carriage | `3/8 camera screw`, `1/4 camera screw` | photo store / AliExpress | | |
| C16 | Zip ties 3 mm | 10 | bearings, cables | `cable tie` | ELEC | | |
| C17 | Rubber feet / pads 20 mm | 4–8 | table feet | `rubber feet`, `silicone bumper` | ELEC | | |

> 1/4"-20 and 3/8"-16 are **inch** (UNC) threads – the same as every tripod and camera.
> If the electronics shops don't stock them, try a hardware shop (search "صامولة بوصة 1/4") or a
> photography shop (look for "1/4 to 3/8 screw adapter" kits – they often include nuts).

## D. Camera side (photography store, not in the list above)

| # | Item | Spec | Qty |
|---|------|------|-----|
| D1 | Ball head or small fluid head | ≥ 5 kg rated, **3/8" thread underneath**, Arca-Swiss clamp | 1 |
| D2 | Arca-Swiss plate for A7 IV | or an L-bracket | 1 |
| D3 | Tripods | any with 1/4" or 3/8" screw. Use 1 at each end of the rail | 1–2 |

## E. 3-D printing

| # | Item | Spec | Qty | Search keywords | Try first | Price | Link |
|---|------|------|-----|-----------------|-----------|-------|------|
| E1 | **PETG filament 1.75 mm** | 1 kg is enough for everything (≈ 600 g used). PETG is stiffer in the sun / car than PLA | 1 | `PETG 1.75`, `PETG filament` | 3DP | | |
| E2 | *(alt.)* PLA+ 1.75 mm | fine for indoor use | 1 | `PLA plus 1.75` | 3DP | | |

---

### Arabic search keywords (if a shop's search is in Arabic)

| English | Arabic |
|---------|--------|
| smooth rod 12 mm | عمود خطي 12 مم |
| linear bearing | رولمان بلي خطي |
| stepper motor | موتور ستيبر |
| timing belt | سير جي تي 2 |
| pulley | بكرة |
| limit switch | ليمت سويتش |
| power adapter | محول باور 12 فولت |
| screw / nut | مسمار / صامولة |
| filament | خيط طباعة ثلاثية الأبعاد |

### Rough budget

Can't confirm prices from here – fill in the table. The most expensive items will be the 2 rods,
4× LM12UU, NEMA17, TMC2209 and ESP32.
