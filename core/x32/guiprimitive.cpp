#include "guiprimitive.h"
#include "kernel.h"

bool GUIPrimitive::IsSupported() {
    message("GUI Primitives: IsSupported(): No Graphics Driver Has Been Loaded");
    return false;
}

int GUIPrimitive::GetMaximum(bool favourDepth) {
    favourDepth = favourDepth;

    message("GUI Primitives: GetMaximum(): No Graphics Driver Has Been Loaded");
    return GUIDRIVERUNSUPPORTED;
}

void GUIPrimitive::SetResolution(uint16_t mode) {
    mode = mode;

    message("GUI Primitives: SetResolution(): No Graphics Driver Has Been Loaded");
}

void GUIPrimitive::DrawPixel(uint16_t fromLeft, uint16_t fromTop, RGB *colour) {
    fromLeft = fromLeft;
    fromTop = fromTop;
    colour = colour;

    message("GUI Primitives: DrawPixel(): No Graphics Driver Has Been Loaded");
}

void GUIPrimitive::DrawLine(Rectangle *rectangle, RGB *colour) {
    rectangle = rectangle;
    colour = colour;

    message("GUI Primitives: DrawLine(): No Graphics Driver Has Been Loaded");
}

void GUIPrimitive::DrawRectangle(Rectangle *rectangle, RGB *colour) {
    rectangle = rectangle;
    colour = colour;

    message("GUI Primitives: DrawRectangle(): No Graphics Driver Has Been Loaded");
}

void GUIPrimitive::DrawChar(Rectangle * rectangle, Font * font, RGB * colour) {
    rectangle = rectangle;
    font = font;
    colour = colour;

    message("GUI Primitives: DrawChar(): No Graphics Driver Has Been Loaded");
}
