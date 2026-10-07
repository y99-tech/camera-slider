// ============================================================
//  Camera Slider - all printable parts (modules)
//  Every module is modelled in its *assembled* position.
//  The *_print() wrappers re-orient a part for printing.
// ============================================================
include <config.scad>

// ------------------------------------------------------------
//  Rod-clamp section shared by both end blocks
//  occupies x = +-end_w/2, y = 0..end_d, z = 0..end_h
//  rods enter from the inner face (y = end_d)
// ------------------------------------------------------------
module end_rod_section() {
    difference() {
        // body with rounded top corners
        hull() {
            translate([-end_w/2, 0, 0]) cube([end_w, end_d, 1]);
            for (sx = [-1, 1])
                translate([sx * (end_w/2 - 8), 0, end_h - 8])
                    rotate([-90, 0, 0]) cylinder(r = 8, h = end_d);
        }

        for (sx = [-1, 1]) {
            x = sx * rod_sp/2;
            // rod socket (blind, rod stops at y = end_d - rod_insert)
            translate([x, end_d - rod_insert, rod_z]) teardrop_y(rod_hole, rod_insert + eps);
            // M4 set-screw from the top + captive nut slot from the outer side
            translate([x, end_d - rod_insert/2, rod_z]) cylinder(d = m4_hole, h = 50);
            translate([x - m4_nut/2 + (sx > 0 ? 0 : -40), end_d - rod_insert/2 - m4_nut/2, rod_z + rod_d/2 + 3])
                cube([m4_nut + 40, m4_nut, m4_nut_t]);
            // weight-saving pocket between rod and edge
        }

        // belt passage (both runs)
        translate([-6, -eps, pulley_z - 11]) cube([12, end_d + 2*eps, 22 + 2]);

        // ---- tripod mounting: captured nuts, inserted from the inner face ----
        // 3/8"-16 in the centre
        translate([0, end_d/2, -eps]) cylinder(d = q38_hole, h = floor_t + 2);
        translate([0, end_d/2, floor_t]) hex(q38_nut, q38_nut_t);
        translate([-q38_nut/2, end_d/2, floor_t]) cube([q38_nut, end_d, q38_nut_t]);
        // two 1/4"-20
        for (sx = [-1, 1]) {
            translate([sx * 35, end_d/2, -eps]) cylinder(d = q14_hole, h = floor_t + 2);
            translate([sx * 35, end_d/2, floor_t]) rotate(30) hex(q14_nut, q14_nut_t);
            translate([sx * 35 - q14_nut/2, end_d/2, floor_t]) cube([q14_nut, end_d, q14_nut_t]);
        }

        // lightening / cable pockets between the tripod nuts and the rods
        for (sx = [-1, 1])
            translate([sx * 21 - 6, -eps, floor_t + 12]) cube([12, end_d + 2*eps, 14]);

        // endstop cable channel along the inner face (top)
        translate([-end_w/2 + 4, end_d - 4, end_h - 3]) cube([end_w - 8, 5, 3 + eps]);
    }
}

// ------------------------------------------------------------
//  MOTOR END   (rods at y = 0..end_d, motor bay at y < 0)
// ------------------------------------------------------------
module motor_end() {
    pt = 5;                         // motor plate thickness
    px = -14;                       // motor plate +X face (motor face sits here)
    union() {
        end_rod_section();
        difference() {
            union() {
                // floor
                translate([-end_w/2, -motor_bay_d, 0]) cube([end_w, motor_bay_d + eps, floor_t]);
                // motor plate
                translate([px - pt, -motor_bay_d, 0]) cube([pt, motor_bay_d + eps, pulley_z + nema_w/2 + 4]);
                // opposite bearing-free cheek (stiffness + belt guard)
                translate([14, -motor_bay_d, 0]) cube([pt, motor_bay_d + eps, pulley_z + 12]);
                // rear wall (electronics box mounts here)
                translate([-end_w/2, -motor_bay_d, 0]) cube([end_w, 5, 60]);
                // gussets
                for (gx = [-end_w/2, end_w/2 - 5])
                    translate([gx, -motor_bay_d, 0]) cube([5, motor_bay_d + eps, 30]);
            }
            // NEMA17 boss + screw holes (screws inserted from the pulley side)
            translate([px - pt - eps, pulley_y_motor, pulley_z]) {
                rotate([0, 90, 0]) cylinder(d = nema_boss, h = pt + 2);
                for (dy = [-1, 1], dz = [-1, 1])
                    translate([0, dy * nema_hole_sp/2, dz * nema_hole_sp/2])
                        rotate([0, 90, 0]) hull() {
                            // slotted 1 mm so belt tension can be trimmed
                            cylinder(d = m3_hole, h = pt + 2);
                            translate([0, 1, 0]) cylinder(d = m3_hole, h = pt + 2);
                        }
            }
            // electronics box screws (M4, 80 mm apart) + cable hole
            for (sx = [-1, 1])
                translate([sx * 40, -motor_bay_d - eps, 35]) rotate([-90, 0, 0]) cylinder(d = m4_hole, h = 10);
            translate([0, -motor_bay_d - eps, 45]) rotate([-90, 0, 0]) cylinder(d = 12, h = 10);
            // floor drain / weight holes
            for (sx = [-1, 1])
                translate([sx * 45, -motor_bay_d/2, -eps]) cylinder(d = 16, h = floor_t + 2);
        }
    }
}

// ------------------------------------------------------------
//  IDLER END  (hand-crank shaft on two 625ZZ bearings)
// ------------------------------------------------------------
module idler_end() {
    wt = 8;                     // wall thickness (holds a 5 mm wide 625ZZ)
    wx = 12;                    // inner face of each wall
    union() {
        end_rod_section();
        difference() {
            union() {
                translate([-end_w/2, -idler_bay_d, 0]) cube([end_w, idler_bay_d + eps, floor_t]);
                for (sx = [-1, 1])
                    translate([sx > 0 ? wx : -wx - wt, -idler_bay_d, 0])
                        hull() {
                            cube([wt, idler_bay_d + eps, pulley_z]);
                            translate([0, idler_bay_d + pulley_y_idler, pulley_z])
                                rotate([0, 90, 0]) cylinder(r = 14, h = wt);
                            translate([0, idler_bay_d - 1, 0]) cube([wt, 1, pulley_z + 14]);
                        }
                translate([-end_w/2, -idler_bay_d, 0]) cube([end_w, 5, 25]);
                for (gx = [-end_w/2, end_w/2 - 5])
                    translate([gx, -idler_bay_d, 0]) cube([5, idler_bay_d + eps, 25]);
            }
            // shaft + bearing seats (bearings pressed in from the outside)
            translate([0, pulley_y_idler, pulley_z]) {
                rotate([0, 90, 0]) cylinder(d = 6, h = 100, center = true);
                for (sx = [-1, 1])
                    translate([sx > 0 ? wx + wt - b625_w : -wx - wt - eps, 0, 0])
                        rotate([0, 90, 0]) cylinder(d = b625_od, h = b625_w + eps);
            }
            for (sx = [-1, 1])
                translate([sx * 45, -idler_bay_d/2, -eps]) cylinder(d = 16, h = floor_t + 2);
        }
    }
}

// ------------------------------------------------------------
//  CARRIAGE  (4x LM12UU, ball-head mount, two belt clamps)
// ------------------------------------------------------------
belt_clamp_y = car_l/2 - 10;     // centre of each belt clamp block
belt_block_bottom = belt_top_z + 0.9;   // belt (1.4 mm) lies just below this face

module carriage() {
    difference() {
        union() {
            // top plate
            translate([-car_w/2, -car_l/2, car_z0])
                minkowski() {
                    cube([car_w - 8, car_l - 8, car_t - 1]);
                    translate([4, 4, 0]) cylinder(r = 4, h = 1, $fn = 24);
                }
            // bearing housings
            for (sx = [-1, 1])
                translate([sx * rod_sp/2, -car_l/2, rod_z])
                    rotate([-90, 0, 0]) cylinder(r = housing_r, h = car_l);
            // web between housing and plate
            for (sx = [-1, 1])
                translate([sx * rod_sp/2 - housing_r, -car_l/2, rod_z])
                    cube([2 * housing_r, car_l, car_z0 - rod_z + 1]);
            // belt clamp blocks
            for (sy = [-1, 1])
                translate([-11, sy * belt_clamp_y - 10, belt_block_bottom])
                    cube([22, 20, car_z0 - belt_block_bottom + 1]);
            // spine joining the two clamp blocks (stiffens the plate)
            translate([-4, -belt_clamp_y, belt_block_bottom + 6])
                cube([8, 2 * belt_clamp_y, car_z0 - belt_block_bottom - 5]);
        }
        // bearing bores (through) - 2 bearings per side, one at each end
        for (sx = [-1, 1])
            translate([sx * rod_sp/2, -car_l/2 - eps, rod_z])
                rotate([-90, 0, 0]) cylinder(d = lm_bore, h = car_l + 2*eps);
        // centre relief so the rod never touches the plastic between bearings
        // zip-tie grooves around each bearing
        for (sx = [-1, 1], sy = [-1, 1])
            translate([sx * rod_sp/2, sy * (car_l/2 - lm_len/2), rod_z])
                rotate([-90, 0, 0])
                    difference() {
                        cylinder(r = housing_r + 2, h = 4, center = true);
                        cylinder(r = housing_r - 1.6, h = 5, center = true);
                    }
        // belt clamp screws: M3 from below, nut dropped in from the top
        for (sy = [-1, 1], sx = [-1, 1]) {
            translate([sx * 7, sy * belt_clamp_y, belt_block_bottom - eps])
                cylinder(d = m3_hole, h = 40);
            translate([sx * 7, sy * belt_clamp_y, belt_block_bottom + 6])
                rotate(30) hex(m3_nut, 40);
        }
        // belt guide groove in the clamp block underside
        for (sy = [-1, 1])
            translate([-belt_w/2 - 0.4, sy * belt_clamp_y - 11, belt_block_bottom - eps])
                cube([belt_w + 0.8, 22, 0.6]);
        // ---- camera / head mounting ----
        // centre 3/8" hole, two 1/4" holes (use with a 3/8" or 1/4" screw from below)
        translate([0, 0, car_z0 - 20]) cylinder(d = 9.8, h = 40);
        for (sx = [-1, 1]) translate([sx * 28, 0, car_z0 - 20]) cylinder(d = 6.6, h = 40);
        // anti-twist slots for heads with locating pins
        for (sy = [-1, 1]) hull()
            for (dy = [0, 8]) translate([0, sy * (16 + dy), car_z0 - 1]) cylinder(d = 5, h = 20);
        // light-weighting windows
        for (sx = [-1, 1], sy = [-1, 1])
            translate([sx * 25, sy * 32, car_z0 - eps])
                cylinder(d = 18, h = car_t + 1);
    }
}

// carriage printed top-down (plate on the bed)
module carriage_print() {
    translate([0, 0, car_z0 + car_t]) rotate([180, 0, 0]) carriage();
}

// ------------------------------------------------------------
//  BELT CLAMP PLATE (x2) - GT2 teeth face up into the belt
// ------------------------------------------------------------
module belt_clamp_plate() {
    t = 4;
    difference() {
        union() {
            translate([-11, -10, 0]) cube([22, 20, t]);
            // GT2 teeth (2 mm pitch) across the belt
            for (i = [-4 : 4])
                translate([-belt_w/2, i * 2 - 0.5, t]) cube([belt_w, 1.0, 0.7]);
            // side walls keep the belt centred
            for (sx = [-1, 1])
                translate([sx > 0 ? belt_w/2 + 0.4 : -belt_w/2 - 0.4 - 1.2, -10, t]) cube([1.2, 20, 1.2]);
        }
        for (sx = [-1, 1]) translate([sx * 7, 0, -eps]) cylinder(d = m3_hole, h = t + 2);
    }
}

// ------------------------------------------------------------
//  QUICK-RELEASE THUMB KNOB (holds an M3 screw head) - for hand mode
// ------------------------------------------------------------
module thumb_knob() {
    difference() {
        union() {
            cylinder(d = 16, h = 7, $fn = 12);
            cylinder(d = 8, h = 10);
        }
        translate([0, 0, -eps]) cylinder(d = 5.8, h = 3.2, $fn = 6 * 4);   // M3 socket head
        translate([0, 0, -eps]) cylinder(d = m3_hole, h = 20);
    }
}

// ------------------------------------------------------------
//  HAND WHEEL for the idler shaft (5 mm shaft, M3 grub screw)
// ------------------------------------------------------------
module hand_wheel() {
    d = 46; t = 8; hub = 16;
    difference() {
        union() {
            // knurled rim
            difference() {
                cylinder(d = d, h = t, $fn = 96);
                for (a = [0 : 10 : 359]) rotate(a) translate([d/2 + 0.6, 0, -eps]) cylinder(d = 3, h = t + 1, $fn = 12);
            }
            cylinder(d = hub, h = t + 8);
        }
        // spokes cut-outs
        difference() {
            translate([0, 0, 3]) cylinder(d = d - 8, h = t);
            cylinder(d = hub, h = 40);
            for (a = [0, 120, 240]) rotate(a) translate([0, -4, 0]) cube([d, 8, 40]);
        }
        // shaft
        translate([0, 0, -eps]) cylinder(d = 5.2, h = 40);
        // M3 grub screw + nut trap
        translate([0, 0, t + 4]) rotate([0, 90, 0]) cylinder(d = m3_hole, h = 20);
        translate([4.5, -m3_nut/2, t + 4 - m3_nut/2 - 3]) cube([m3_nut_t, m3_nut, m3_nut + 6]);
        // crank handle hole (M5 bolt + printed sleeve / spinner)
        translate([d/2 - 7, 0, -eps]) cylinder(d = m5_hole, h = 40);
    }
}

// spinner sleeve for the crank handle (slides over an M5x30 bolt)
module crank_spinner() {
    difference() {
        cylinder(d = 12, h = 22);
        translate([0, 0, -eps]) cylinder(d = 5.8, h = 30);
    }
}

// ------------------------------------------------------------
//  ENDSTOP CLIP - clamps on a 12 mm rod, holds a KW11 micro switch
// ------------------------------------------------------------
module endstop_clip() {
    w = 10;
    difference() {
        union() {
            cylinder(d = rod_d + 8, h = w);
            // arm that carries the switch
            translate([0, -3, 0]) cube([rod_d/2 + 18, 6, w]);
            // clamp ears
            translate([-rod_d/2 - 10, -5, 0]) cube([10, 10, w]);
        }
        translate([0, 0, -eps]) cylinder(d = rod_hole, h = w + 1);
        translate([-rod_d/2 - 11, -0.75, -eps]) cube([12, 1.5, w + 1]);  // split
        translate([-rod_d/2 - 5, 10, w/2]) rotate([90, 0, 0]) cylinder(d = m3_hole, h = 20);
        // KW11 switch holes (M2, 9.5 mm apart)
        for (dx = [0, 9.5])
            translate([rod_d/2 + 6 + dx, 10, w/2]) rotate([90, 0, 0]) cylinder(d = 2.2, h = 20);
    }
}

// ------------------------------------------------------------
//  TABLE FOOT (x2..4) - screws into an end block 1/4" nut
//  wide stance + recess for a rubber pad
// ------------------------------------------------------------
module table_foot() {
    l = 70; w = 34; h = 8;
    difference() {
        hull() for (sx = [-1, 1]) translate([sx * (l/2 - w/2), 0, 0]) cylinder(d = w, h = h);
        // 1/4" screw, head recessed from below
        translate([0, 0, -eps]) cylinder(d = q14_hole, h = h + 1);
        translate([0, 0, -eps]) cylinder(d = 13, h = 4.5);
        // rubber pad recesses
        for (sx = [-1, 1]) translate([sx * (l/2 - w/2), 0, -eps]) cylinder(d = 20, h = 1.2);
    }
}

// ------------------------------------------------------------
//  ELECTRONICS BOX (ESP32 + TMC2209 + buck + switch + DC jack)
//  bolts to the motor-end rear wall with 2x M4 (80 mm apart)
// ------------------------------------------------------------
box_l = 110; box_w = 76; box_h = 42; box_wall = 2.4; box_r = 5;

module rounded_box(l, w, h, r) {
    hull() for (sx = [-1, 1], sy = [-1, 1])
        translate([sx * (l/2 - r), sy * (w/2 - r), 0]) cylinder(r = r, h = h);
}

module elec_box() {
    difference() {
        union() {
            difference() {
                rounded_box(box_l, box_w, box_h, box_r);
                translate([0, 0, box_wall]) rounded_box(box_l - 2*box_wall, box_w - 2*box_wall, box_h, box_r - box_wall);
            }
            // lid screw bosses
            for (sx = [-1, 1], sy = [-1, 1])
                translate([sx * (box_l/2 - 6), sy * (box_w/2 - 6), 0]) cylinder(d = 8, h = box_h);
            // standoffs for a 70x50 mm perfboard (holes 2.5 mm, 64x44 pattern)
            for (sx = [-1, 1], sy = [-1, 1])
                translate([sx * 32 - 8, sy * 22, 0]) cylinder(d = 6, h = box_wall + 5);
        }
        for (sx = [-1, 1], sy = [-1, 1]) {
            translate([sx * (box_l/2 - 6), sy * (box_w/2 - 6), box_wall]) cylinder(d = 2.6, h = box_h);
            translate([sx * 32 - 8, sy * 22, 1]) cylinder(d = 2.4, h = 20);
        }
        // M4 mounting holes in the floor (to the motor-end rear wall)
        for (sx = [-1, 1]) translate([sx * 40, 0, -eps]) cylinder(d = m4_hole, h = box_wall + 1);
        // cable exit to the slider (lines up with the 12 mm hole in the rear wall)
        translate([0, 10, -eps]) cylinder(d = 11, h = box_wall + 1);
        // -X wall: DC jack (8 mm) + rocker switch (19x13)
        translate([-box_l/2 - 1, -18, 20]) rotate([0, 90, 0]) cylinder(d = 8, h = 10);
        translate([-box_l/2 - 1, 6, 20 - 6.5]) cube([10, 19.5, 13.2]);
        // +X wall: USB access for the ESP32 (flashing / power)
        translate([box_l/2 - 5, -6, box_wall + 9]) cube([10, 12, 8]);
        // +Y wall: 3 cable holes (motor, endstops, shutter)
        for (dx = [-25, 0, 25]) translate([dx, box_w/2 - 5, 24]) rotate([-90, 0, 0]) cylinder(d = 7, h = 10);
        // vents
        for (i = [-3 : 3]) translate([i * 10 - 1.2, -box_w/2 - 1, box_h - 14]) cube([2.4, 10, 9]);
    }
}

module elec_lid() {
    t = 2.4;
    difference() {
        union() {
            rounded_box(box_l, box_w, t, box_r);
            // locating lip
            translate([0, 0, t]) difference() {
                rounded_box(box_l - 2*box_wall - 0.6, box_w - 2*box_wall - 0.6, 2, box_r - box_wall);
                translate([0, 0, -eps]) rounded_box(box_l - 2*box_wall - 3.6, box_w - 2*box_wall - 3.6, 3, 1);
                for (sx = [-1, 1], sy = [-1, 1])
                    translate([sx * (box_l/2 - 6), sy * (box_w/2 - 6), -eps]) cylinder(d = 9, h = 3);
            }
        }
        for (sx = [-1, 1], sy = [-1, 1])
            translate([sx * (box_l/2 - 6), sy * (box_w/2 - 6), -eps]) {
                cylinder(d = 3.4, h = 10);
                cylinder(d = 6.2, h = 1.4);
            }
        // rotary encoder (7 mm thread) + anti-rotation tab
        translate([25, 0, -eps]) cylinder(d = 7.4, h = 10);
        translate([25 + 7.8, -1.3, -eps]) cube([1.6, 2.6, 10]);
        // two push buttons (12 mm tactile caps / 12 mm panel buttons)
        for (dx = [-10, -32]) translate([dx, 0, -eps]) cylinder(d = 12.4, h = 10);
        // status LED (5 mm)
        translate([45, 22, -eps]) cylinder(d = 5.2, h = 10);
        // label recess
        translate([-50, -30, -eps]) cube([40, 8, 0.6]);
    }
}

// lid is printed with its outer face down - this is already that orientation
module elec_lid_print() { elec_lid(); }

// ------------------------------------------------------------
//  ENCODER KNOB (for a KY-040 / EC11 D-shaft)
// ------------------------------------------------------------
module encoder_knob() {
    difference() {
        union() {
            cylinder(d = 24, h = 14, $fn = 30);
            translate([0, 9, 14 - eps]) cylinder(d = 2, h = 0.6);  // pointer dot
        }
        // ribs
        for (a = [0 : 15 : 359]) rotate(a) translate([12.4, 0, -eps]) cylinder(d = 1.6, h = 20, $fn = 8);
        // D shaft 6 mm with 4.5 flat
        translate([0, 0, -eps]) intersection() {
            cylinder(d = 6.2, h = 11);
            translate([-3.1, -3.1 + 1.5 - 0.1, 0]) cube([6.2, 6.2, 11]);
        }
    }
}
