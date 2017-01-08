#ifndef GUIPRIMITIVE
#define GUIPRIMITIVE

#include <stdint.h>
#include "font.h"

#define GUIDRIVERUNSUPPORTED -1

class GUIPrimitive {

public:

    typedef struct RGB {
        uint8_t Red;
        uint8_t Green;
        uint8_t Blue;
    } RGB;

    typedef struct Rectangle {
        uint16_t FromLeft;
        uint16_t FromTop;
        uint16_t ToLeft;
        uint16_t ToTop;
    } Rectangle;

    //Detection and Enablement
    virtual bool IsSupported(); //Graphics hardware supports this?
    virtual int GetMaximum(bool favourDepth = true); //Get maximum supported
    virtual void SetResolution(uint16_t mode);

    virtual void DrawPixel(uint16_t fromLeft, uint16_t fromTop, RGB * colour);
    virtual void DrawLine(Rectangle * rectangle, RGB * colour);
    virtual void DrawRectangle(Rectangle * rectangle, RGB * colour);
    virtual void DrawChar(Rectangle * rectangle, Font * font, RGB * colour);

};

#endif // GUIPRIMITIVE

