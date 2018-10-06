#ifndef RGBQUAD_H
#define RGBQUAD_H

class RGBQUAD {

public:
    RGBQUAD();
    RGBQUAD(unsigned char blue, unsigned char green, unsigned char red);

    unsigned char  rgbBlue;
    unsigned char  rgbGreen;
    unsigned char  rgbRed;
    unsigned char  rgbReserved;
};
#endif // RGBQUAD_H
