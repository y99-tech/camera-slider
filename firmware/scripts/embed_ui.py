# PlatformIO pre-build script: turns ui/index.html into src/web_ui.h
# so the same page is served by the ESP32 and can also be opened
# directly on a phone/PC for Bluetooth control.
import os

try:
    Import("env")  # noqa: F821  (provided by PlatformIO)
    root = env.subst("$PROJECT_DIR")  # noqa: F821
except NameError:
    root = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))

src = os.path.join(root, "ui", "index.html")
dst = os.path.join(root, "src", "web_ui.h")

with open(src, "r", encoding="utf-8") as f:
    html = f.read()

out = (
    "// AUTO-GENERATED from ui/index.html by scripts/embed_ui.py - do not edit\n"
    "#pragma once\n"
    "#include <pgmspace.h>\n"
    'static const char WEB_UI_HTML[] PROGMEM = R"SLIDERUI(' + html + ')SLIDERUI";\n'
)

old = None
if os.path.exists(dst):
    with open(dst, "r", encoding="utf-8") as f:
        old = f.read()
if old != out:
    with open(dst, "w", encoding="utf-8") as f:
        f.write(out)
    print("embed_ui: wrote", dst)
