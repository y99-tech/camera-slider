#!/usr/bin/env bash
# Checks that assembled parts don't collide (each test must come out "CLEAR")
# and that every STL fits the Ender-3 V3 bed (220 x 220 x 250 mm).
set -uo pipefail
cd "$(dirname "$0")"
names=("" "carriage vs rods" "carriage vs belt" "carriage at end of travel vs end block"
       "motor vs motor pod + end block" "end block vs pod (touch only)" "pulleys vs pods/blocks"
       "rollers vs carriage" "rollers vs rods (touch only)" "belt vs pods/blocks" "axles vs rods"
       "rollers vs belt" "electronics box vs motor pod" "hand wheel vs crank pod + end block")
tmp=$(mktemp -d)
for t in 1 2 3 4 6 7 8 9 10 11 12 13; do
  out=$(openscad -o "$tmp/t.stl" -D test=$t check_fit.scad 2>&1)   # exits non-zero when the result is empty
  if grep -q "empty" <<<"$out"; then r=CLEAR; else r="OVERLAP  <-- check!"; fi
  printf "%2d  %-42s %s\n" "$t" "${names[$t]}" "$r"
done
rm -rf "$tmp"
echo
python3 - <<'PY'
import glob, os
for p in sorted(glob.glob('stl/*.stl')):
    mn=[1e9]*3; mx=[-1e9]*3
    for line in open(p):
        if line.lstrip().startswith('vertex'):
            v=list(map(float,line.split()[1:]))
            mn=[min(a,b) for a,b in zip(mn,v)]; mx=[max(a,b) for a,b in zip(mx,v)]
    d=[b-a for a,b in zip(mn,mx)]
    ok = d[0]<=220 and d[1]<=220 and d[2]<=250
    print(f"{os.path.basename(p):22s} {d[0]:6.1f} x {d[1]:6.1f} x {d[2]:6.1f} mm  {'fits' if ok else 'TOO BIG'}")
PY
