// Select a single part for export:  openscad -D 'part="carriage_print"' -o x.stl print.scad
include <parts.scad>
part = "carriage_print";

if (part == "end_block")        end_block();
if (part == "motor_pod")        motor_pod();
if (part == "crank_pod")        crank_pod();
if (part == "spring_seat")      spring_seat();
if (part == "carriage_print")   carriage_print();
if (part == "belt_clamp_plate") belt_clamp_plate();
if (part == "thumb_knob")       thumb_knob();
if (part == "hand_wheel")       hand_wheel();
if (part == "crank_spinner")    crank_spinner();
if (part == "roller_spacer")    roller_spacer();
if (part == "table_foot")       table_foot();
if (part == "elec_box")         elec_box();
if (part == "elec_lid_print")   elec_lid_print();
if (part == "encoder_knob")     encoder_knob();
