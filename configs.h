#ifndef OTHERFUNCS_H
#define OTHERFUNCS_H

#include "rgbquad.h"

typedef struct {
    unsigned int thickLine;
    unsigned int starVertexNum;
    RGBQUAD color;
} PaintConfigs;

typedef struct {
    unsigned int width;
    unsigned int height;
    unsigned int bitcount;
    unsigned int version;
} ImageConfigs;

#endif // OTHERFUNCS_H
