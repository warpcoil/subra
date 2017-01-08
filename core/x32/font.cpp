#include "font.h"
#include "kernel.h"
#include "oslib/error.h"

/* Basic/Primitive Kernel Font
 * Available in 8, 12 and 16pt
 * Whilst this code remains GPL
 * The fonts (based off of ttf and converted remains under their own license)
 *
 *
 * KERNEL PANIC NBP
 * made by Nate Halley
 * using Fontstruct
 * Version 1.0
 * Date: 16 January 2013
 *
 * Kernel Panic is Creative Commons (by-sa) Attribution Share Alike. That means it's free to download and use. You can also upload it to another website but only as long as you give me credit for making it. You can even make changes to it as long as you give me credit for making the first version and license your new version as CC-BY-SA too.
 * For more information, go to:
 * http://creativecommons.org/licenses/by-sa/3.0/
 * */

int Font::getLocation(uint8_t character, bitmap_font * font) {

    int location = 0;

    if (font->Index[location] == character) {
        return location;
    }

    for (location=0; location<font->Chars; location++) {
        if (font->Index[location] == character) {
            return location;
        }
    }

    return NOFONTCHARACTER;

}

int Font::getChar(Point size, uint8_t character, Font::Character * where) {

    if (!where)
        return NOFONTPLACEMENT;

    struct bitmap_font * font;

    switch (size) {
    case 0:
        font = (bitmap_font *)&kernelfont8;
        break;
    case 1:
        font = (bitmap_font *)&kernelfont12;
        break;
    case 2:
        font = (bitmap_font *)&kernelfont16;
        break;
    default:
        return NOFONTSIZE;
    }

    //Check that the character does not exceed bounds of bitmap array
    if (character > font->Chars) {
        return NOFONTCHARACTER;
    }

    //Index lookup
    int location = getLocation(character, font);

    //Check exists
    if (location < 0) {
        return location; //Contains an error code
    }

    where->Width = font->Widths[location];
    where->Bitmap = &font->Bitmap[location * font->Height];
    where->NumBytes = font->Height * font->Widths[location];

    return NOERROR;

}
