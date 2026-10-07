// ============================================================
//  Camera Slider - shared parameters
//  Units: mm.  Axes: X = across the rail, Y = along the rail, Z = up.
//  Edit values here, then re-run ./cad/export_stl.sh
// ============================================================

$fn = 64;

// ---------- Rails: 4 rods = 2 "twin-rod" tracks ----------
// Each track is two parallel rods; 608ZZ rollers ride in the groove
// between them (2 on top + 1 preloaded underneath per track).
rod_d        = 10;      // smooth rod diameter (4 rods)
rod_hole     = 10.25;   // printed hole for the rod (tune to your printer)
track_sp     = 100;     // centre-to-centre distance of the two tracks
pair_sp      = 16;      // centre-to-centre distance of the 2 rods in one track
rod_z        = 45;      // rod centre height above the ground plane
rail_len     = 800;     // rod length (only used for the assembly preview)
rod_insert   = 26;      // how deep the rods go into each end block

// rod X positions (all four)
rod_xs = [-track_sp/2 - pair_sp/2, -track_sp/2 + pair_sp/2,
           track_sp/2 - pair_sp/2,  track_sp/2 + pair_sp/2];

// ---------- Rollers (608ZZ skate bearings) ----------
roller_od    = 22;
roller_w     = 7;
axle_d       = 8;       // M8 bolt
// height of a roller centre above (top) / below (bottom) the rod centres
roller_dz    = sqrt(pow(roller_od/2 + rod_d/2, 2) - pow(pair_sp/2, 2));
roller_y     = 42;      // top rollers at +-roller_y from carriage centre

// ---------- Motor / belt ----------
nema_w       = 42.3;    // NEMA17 body
nema_hole_sp = 31;
nema_boss    = 23;      // clearance for the 22 mm centring boss
pulley_z     = rod_z - 14;   // height of the pulley axis (both ends)
pulley_pd    = 12.73;        // GT2 20T pitch diameter
belt_w       = 6;
belt_top_z   = pulley_z + pulley_pd/2;   // centre of the upper belt run

// ---------- End blocks ----------
end_w        = 150;     // width across X
end_d        = 30;      // depth of the rod-clamp section (Y)
end_h        = rod_z + 20;
floor_t      = 6;
motor_bay_d  = 50;      // outboard section on the motor end
idler_bay_d  = 40;      // outboard section on the idler end
pulley_y_motor = -24;   // pulley axis Y position (outboard, negative Y)
pulley_y_idler = -20;

// ---------- Carriage ----------
car_w        = 144;
car_l        = 120;
car_t        = 8;
car_z0       = rod_z + roller_dz + roller_od/2 + 2.5;   // bottom of the top plate
cheek_t      = 6;
cheek_in     = track_sp/2 - pair_sp/2 - rod_d/2 - 1;    // inner cheek outer face (|x|)
cheek_out    = track_sp/2 + pair_sp/2 + rod_d/2 + 1;    // outer cheek inner face (|x|)
cheek_bot    = rod_z - roller_dz - 9;                   // bottom of the cheeks

// ---------- Hardware ----------
m3_hole  = 3.3;   m3_nut  = 6.4;  m3_nut_t = 2.6;
m4_hole  = 4.4;   m4_nut  = 7.3;  m4_nut_t = 3.4;
m5_hole  = 5.4;
// Camera threads (nuts are captured in the end blocks)
q14_hole = 6.8;   q14_nut = 11.5; q14_nut_t = 6.0;   // 1/4"-20 nut, 7/16" AF
q38_hole = 9.9;   q38_nut = 14.7; q38_nut_t = 8.8;   // 3/8"-16 nut, 9/16" AF
b625_od  = 16.15; b625_w  = 5;                       // 625ZZ bearing

eps = 0.01;

// ---------- Helpers ----------
// hexagon given across-flats size
module hex(af, h) { cylinder(d = af / cos(30), h = h, $fn = 6); }

// horizontal hole along Y printable without supports (point at +Z)
// down = true puts the point at -Z (for parts printed upside down)
// clip  = how far (mm) the point may stick out beyond the circle
module teardrop_y(d, l, center = false, down = false, clip = 100) {
    translate([0, center ? l/2 : l, 0])
    rotate([90, 0, 0])
    mirror([0, down ? 1 : 0])
    linear_extrude(l)
    union() {
        circle(d = d);
        intersection() {
            rotate(45) square(d/2);
            translate([-d, 0]) square([2*d, d/2 + clip]);
        }
    }
}

// horizontal hole along X printable without supports (point at +Z)
module teardrop_x(d, l, center = false) {
    rotate([0, 0, -90]) teardrop_y(d, l, center);
}
