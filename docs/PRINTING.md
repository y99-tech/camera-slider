# Printing (Creality Ender-3 V3 / V3 SE / V3 KE)

Every part fits the 220 × 220 mm bed (`cad/check_fit.sh` checks it). Print each STL in the orientation
it's already in. **No supports are needed.**

## Material

**PETG** (stiff, doesn't creep under the camera, OK in a hot car). PLA+ is fine for indoor use.
Filament sources are in [SHOPPING_LIST.md](SHOPPING_LIST.md) (E1).

| Setting | PETG | PLA+ |
|---------|------|------|
| Nozzle / bed | 235–245 °C / 70–80 °C | 210–220 °C / 60 °C |
| Layer height | 0.2 mm | 0.2 mm |
| Walls | **4** | 4 |
| Top / bottom | 5 / 5 | 5 / 5 |
| Fan | 30–50 % | 100 % |
| Supports | none | none |
| Brim | 5 mm on the big parts (PETG can lift at the corners) | – |

## Parts and print order

Print them in this order. The first two let you test the fit of your rods, nuts and bearings before
the long prints.

| # | STL | Qty | Infill | Orientation / notes | ≈ time |
|---|-----|-----|--------|---------------------|--------|
| 1 | `roller_spacer.stl` | **12** | 100 % | lip up. Test: must slide on an M8 bolt | 1 h all |
| 2 | `end_block.stl` | **2** | 40 % gyroid | flat bottom on the bed. **Test rods + nuts in the first one** | 4 h each |
| 3 | `carriage.stl` | 1 | 40 % gyroid | already upside down (top plate on the bed) | 6 h |
| 4 | `motor_pod.stl` | 1 | 35 % | floor on the bed | 5 h |
| 5 | `crank_pod.stl` | 1 | 35 % | floor on the bed | 2.5 h |
| 6 | `spring_seat.stl` | 4 | 100 % | | 10 min |
| 7 | `belt_clamp_plate.stl` | 2 | 100 % | teeth up | 20 min |
| 8 | `hand_wheel.stl` + `crank_spinner.stl` | 1 + 1 | 30 % | | 1.5 h |
| 9 | `thumb_knob.stl` | 1 | 30 % | | 15 min |
| 10 | `elec_box.stl` | 1 | 20 % | | 4 h |
| 11 | `elec_lid.stl` | 1 | 20 % | outside face on the bed | 1 h |
| 12 | `encoder_knob.stl` | 1 | 30 % | optional | 15 min |
| 13 | `table_foot.stl` | 2 | 30 % | optional | 1 h |

Total ≈ **700–800 g** PETG and ≈ 35 h of printing.

## Fit tests (do them on the first end block)

| Check | Good | Fix |
|-------|------|-----|
| 10 mm rod in its socket | slides in with a push, no wobble | `rod_hole` 10.25 → 10.35 (tight) or 10.15 (loose) |
| M4 nut in the hex pockets | presses in | `m4_nut` 7.3 → 7.5 |
| 1/4" & 3/8" nuts | slide in from the inner face | `q14_nut`, `q38_nut` |
| M8 bolt through the carriage cheeks | snug | drill 8.5 mm by hand. Don't widen it in CAD |
| bed spring in its pocket (Ø9) | drops in freely | `spring_d` |
| micro switch in its pocket | push fit | sand the pocket |

After changing `cad/config.scad`, run `./cad/export_stl.sh` (needs [OpenSCAD](https://openscad.org)), then
`./cad/check_fit.sh`.

## Other rod lengths / sizes

* **Length:** nothing to reprint. Set the travel in the app (*Settings → Travel*) or with `SET travel <mm>` + `SAVE`.
  Formula and belt lengths are in [DIMENSIONS.md](DIMENSIONS.md).
* **Different rod diameter or gap:** change `rod_d` / `rod_hole` / `pair_sp`. The roller height, carriage
  height and cheek positions all follow automatically. Then re-export and run `check_fit.sh`.
