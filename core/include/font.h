#ifndef FONT
#define FONT

#include <stdint.h>

#define NOERROR 0
#define NOFONTPLACEMENT -1
#define NOFONTSIZE -2
#define NOFONTCHARACTER - 3

#include "fontc.h"

class Font {

private:
    int getLocation(uint8_t letter, bitmap_font * font);

public:

    enum Point {
        Point8 = 0,
        Point12 = 1,
        Point16 = 2
    };

    typedef struct Character {
        uint8_t Width; /* Height is specified by Point [fixed], Width is variable */
        const unsigned char * Bitmap;
        uint8_t NumBytes;
    } Character;

    int getChar(Point size, uint8_t character, Font::Character * where);

};

#endif // FONT

