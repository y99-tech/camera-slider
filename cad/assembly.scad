// Full assembly preview (not for printing)
include <parts.scad>
car_pos = 0;   // carriage position along the rail (mm, 0 = centre)

ins = end_d - rod_insert;          // rod end inside the block
span = rail_len - 2 * ins;         // distance between the two block origins
y_m = -span/2;                     // motor end origin
y_i =  span/2;                     // idler end origin (mirrored)

color("SteelBlue") translate([0, y_m, 0]) motor_end();
color("SteelBlue") translate([0, y_i, 0]) mirror([0, 1, 0]) idler_end();
// rods
color("Silver") for (sx = [-1, 1])
    translate([sx * rod_sp/2, -rail_len/2, rod_z]) rotate([-90, 0, 0]) cylinder(d = rod_d, h = rail_len);
// carriage + bearings
translate([0, car_pos, 0]) {
    color("Orange") carriage();
    color("Silver") for (sx = [-1, 1], sy = [-1, 1])
        translate([sx * rod_sp/2, sy * (car_l/2 - lm_len/2), rod_z]) rotate([-90, 0, 0])
            difference() { cylinder(d = lm_od, h = lm_len, center = true); cylinder(d = rod_d, h = lm_len + 1, center = true); }
    color("DarkOrange") for (sy = [-1, 1]) translate([0, sy * belt_clamp_y, belt_block_bottom - 1.6 - 4]) belt_clamp_plate();
    // ball head + camera (A7 IV + 24-70 GM II, approximate)
    color("DimGray") translate([0, 0, car_z0 + car_t]) cylinder(d = 40, h = 70);
    color("#222") translate([-65, -35, car_z0 + car_t + 70]) cube([131, 80, 96]);
    color("#111") translate([0, -35, car_z0 + car_t + 70 + 48]) rotate([90, 0, 0]) cylinder(d = 88, h = 120);
}
// motor
color("#333") translate([-14 - 5, y_m + pulley_y_motor, pulley_z]) rotate([0, -90, 0]) translate([-nema_w/2, -nema_w/2, 0]) cube([nema_w, nema_w, 40]);
// pulleys
for (yy = [y_m + pulley_y_motor, y_i - pulley_y_idler])
    color("Gold") translate([-8, yy, pulley_z]) rotate([0, 90, 0]) cylinder(d = 16, h = 16);
// belt (simplified)
color("#111") {
    pm = y_m + pulley_y_motor; pi = y_i - pulley_y_idler;
    translate([-3, pm, belt_top_z - 0.7]) cube([6, car_pos - belt_clamp_y - pm + 10, 1.4]);
    translate([-3, car_pos + belt_clamp_y - 10, belt_top_z - 0.7]) cube([6, pi - (car_pos + belt_clamp_y - 10), 1.4]);
    translate([-3, pm, pulley_z - pulley_pd/2 - 0.7]) cube([6, pi - pm, 1.4]);
}
// hand wheel
color("Tomato") translate([22, y_i - pulley_y_idler, pulley_z]) rotate([0, 90, 0]) hand_wheel();
// electronics box on the motor-end rear wall
color("SlateGray") translate([0, y_m - motor_bay_d, 40]) rotate([90, 0, 0]) elec_box();
