// Interference checks - run ./check_fit.sh
include <parts.scad>
test = 0;
ins = end_d - rod_insert; span = rail_len + 2 * ins; y_m = -span/2; y_i = span/2;
module rods() for (x = rod_xs) translate([x, -rail_len/2, rod_z]) rotate([-90, 0, 0]) cylinder(d = rod_d, h = rail_len);
module roller() rotate([0, 90, 0]) difference() { cylinder(d = roller_od - 0.05, h = roller_w, center = true); cylinder(d = 8, h = 8, center = true); }
module rollers() for (sx = [-1, 1]) { for (sy = [-1, 1]) translate([sx * track_sp/2, sy * roller_y, top_axle_z]) roller(); translate([sx * track_sp/2, 0, bot_axle_z]) roller(); }
module axles() for (sx = [-1, 1]) for (p = [[-roller_y, top_axle_z], [roller_y, top_axle_z], [0, bot_axle_z]])
        translate([sx * track_sp/2, p[0], p[1]]) rotate([0, 90, 0]) cylinder(d = 7.9, h = 2 * (cheek_out + cheek_t + boss_w - track_sp/2) + 10, center = true);
module belt(cp) { pm = y_m + pulley_y_motor; pi = y_i - pulley_y_idler;
    translate([-3, pm, belt_top_z - 0.7]) cube([6, cp - belt_clamp_y - pm + 10 - 0.2, 1.4]);
    translate([-3, pm, pulley_z - pulley_pd/2 - 0.7]) cube([6, pi - pm, 1.4]); }
module motor() translate([-14 - 5 - 0.05, y_m + pulley_y_motor, pulley_z]) rotate([0, -90, 0]) translate([-nema_w/2, -nema_w/2, 0]) cube([nema_w, nema_w, 40]);
module pulleys() for (yy = [y_m + pulley_y_motor, y_i - pulley_y_idler]) translate([-8, yy, pulley_z]) rotate([0, 90, 0]) cylinder(d = 16, h = 16);
// carriage at its furthest position toward the motor end: front face 1 mm from the end block
far = y_m + end_d + 1 + car_l/2;
if (test == 1) intersection() { carriage(); rods(); }
if (test == 2) intersection() { carriage(); belt(0); }
if (test == 3) intersection() { translate([0, far, 0]) carriage(); translate([0, y_m, 0]) end_block(); }
if (test == 4) intersection() { motor(); translate([0, y_m, 0]) union() { motor_pod(); end_block(); } }
if (test == 5) intersection() { translate([0, y_m, 0]) end_block(); translate([0, y_m, 0]) motor_pod(); }
if (test == 6) intersection() { pulleys(); union() { translate([0, y_m, 0]) union() { motor_pod(); end_block(); } translate([0, y_i, 0]) rotate([0,0,180]) union() { crank_pod(); end_block(); } } }
if (test == 7) intersection() { rollers(); carriage(); }
if (test == 8) intersection() { rollers(); rods(); }
if (test == 9) intersection() { belt(0); union() { translate([0, y_m, 0]) union() { motor_pod(); end_block(); } translate([0, y_i, 0]) rotate([0,0,180]) union() { crank_pod(); end_block(); } } }
if (test == 10) intersection() { axles(); rods(); }
if (test == 11) intersection() { rollers(); belt(0); }
if (test == 12) intersection() { translate([0, y_m - motor_bay_d - 0.05, box_z]) rotate([90, 0, 0]) elec_box(); translate([0, y_m, 0]) motor_pod(); }
if (test == 13) intersection() { translate([0, y_i, 0]) rotate([0,0,180]) union() { crank_pod(); end_block(); } translate([wheel_x, y_i - pulley_y_idler, pulley_z]) rotate([0, 90, 0]) hand_wheel(); }
