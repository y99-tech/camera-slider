# Printing (Creality Ender-3 V3 / V3 SE / V3 KE)

All parts fit the 220 × 220 mm bed. Print them in the orientation the STL is already in.

## Material

**PETG** is recommended (stiffer, doesn't creep under the camera weight, survives a hot car).
PLA+ is OK for indoor use.

| Setting | PETG | PLA+ |
|---------|------|------|
| Nozzle / bed | 235–245 °C / 70–80 °C | 210–220 °C / 60 °C |
| Layer height | 0.2 mm | 0.2 mm |
| Walls (perimeters) | **4** | 4 |
| Top / bottom layers | 5 / 5 | 5 / 5 |
| Fan | 30–50 % | 100 % |
| Supports | **none needed** | none |

## Parts

| STL | Qty | Infill | Notes |
|-----|-----|--------|-------|
| `motor_end.stl`        | 1 | 40 % gyroid | flat on bed. Longest print (~6–7 h) |
| `idler_end.stl`        | 1 | 40 % gyroid | flat on bed |
| `carriage.stl`         | 1 | 40 % gyroid | already upside down (top plate on bed) |
| `belt_clamp_plate.stl` | 2 | 100 % | teeth up |
| `thumb_knob.stl`       | 1 | 30 % | quick-release for hand mode |
| `hand_wheel.stl`       | 1 | 30 % | crank on the idler shaft |
| `crank_spinner.stl`    | 1 | 30 % | spins on an M5×30 bolt |
| `roller_spacer.stl`    | **12** | 100 % | 2 per roller, lip toward the bearing |
| `table_foot.stl`       | 2 | 30 % | optional – wider stance on a table |
| `elec_box.stl`         | 1 | 20 % | |
| `elec_lid.stl`         | 1 | 20 % | outside face on the bed |
| `encoder_knob.stl`     | 1 | 30 % | optional |

Total ≈ 550–650 g of filament.

## Fit tuning

Before the big parts, print a small test: a 15 mm cube with a 10.25 mm hole (or just print one end block
first) and check your rods fit.

* Rod too loose/tight → change `rod_hole` in `cad/config.scad`.
* M8 bolts tight in the cheeks → drill the holes with an 8.5 mm drill (don't widen them in CAD, they should be snug).
* Your rods aren't 10 mm, or you want a different gap between the 2 rods of a track → change `rod_d` and `pair_sp`.
  Roller height (`roller_dz`) and the carriage height follow automatically.
* Nuts don't fit → adjust `m3_nut`, `m4_nut`, `q14_nut`, `q38_nut`.

Then run `./cad/export_stl.sh` (needs [OpenSCAD](https://openscad.org)) to regenerate all STLs.

## Different rail length / rods

* Rail length: just cut the rods. Set the firmware travel to match: `SET travel <mm>`, `SAVE`.
  Usable travel ≈ rod length − 52 mm (inside the blocks) − 120 mm (carriage) − ~20 mm (endstop levers).
  800 mm rods → ~600 mm travel. 1000 mm rods → ~800 mm.
* With 4 × 10 mm rods the load is shared by all four. With the A7 IV + 24-70 GM II in the middle and a
  tripod at each end, they bend about **0.5 mm at 800 mm** and about **1 mm at 1000 mm**. That's fine for video.
  For longer rails, add a third tripod/light stand in the middle under a rod clamp, or use thicker rods.
