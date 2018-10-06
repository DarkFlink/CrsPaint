#include "rgbquad.h"

RGBQUAD::RGBQUAD() {
    rgbReserved = 0;
}

RGBQUAD::RGBQUAD(unsigned char blue, unsigned char green, unsigned char red) {
    rgbBlue = blue;
    rgbGreen = green;
    rgbRed = red;
    rgbReserved = 0;
}
