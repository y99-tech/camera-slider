// Exploded view of the modules (not for printing)
include <parts.scad>
gap = 45;
// motor end: pod | end block
color("CadetBlue") translate([0, -gap, 0]) motor_pod();
color("SteelBlue") end_block();
color("SlateGray") translate([0, -gap - motor_bay_d - 30, 40]) rotate([90, 0, 0]) elec_box();
// carriage lifted, with the module interface plate area marked
color("Orange") translate([0, 160, 30]) carriage();
// crank end: end block | pod (turned 180)
color("SteelBlue") translate([0, 330, 0]) rotate([0, 0, 180]) end_block();
color("CadetBlue") translate([0, 330 + gap, 0]) rotate([0, 0, 180]) crank_pod();
color("Tomato") translate([wheel_x, 330 + gap - pulley_y_idler, pulley_z]) rotate([0, 90, 0]) hand_wheel();
// short rod stubs to show the tracks
color("Silver") for (x = rod_xs) translate([x, 10, rod_z]) rotate([-90, 0, 0]) cylinder(d = rod_d, h = 290);
