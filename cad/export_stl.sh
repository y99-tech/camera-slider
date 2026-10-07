#!/usr/bin/env bash
# Export every printable part to cad/stl/ (needs OpenSCAD on PATH)
set -euo pipefail
cd "$(dirname "$0")"
mkdir -p stl
parts=(motor_end idler_end carriage_print belt_clamp_plate thumb_knob hand_wheel
       crank_spinner endstop_clip table_foot elec_box elec_lid_print encoder_knob)
for p in "${parts[@]}"; do
  out="stl/${p%_print}.stl"
  echo "-> $out"
  openscad -q -o "$out" -D "part=\"$p\"" print.scad
done
