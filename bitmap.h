#ifndef BITMAP_H
#define BITMAP_H

#include <cmath>
#include <cstring>
#include <fstream>
#include "configs.h"
#include "rgbquad.h"
#include "point.h"

#define ANGLE_72GRAD 1.25664

using namespace std;

typedef struct {
    int ciexyzX;
    int ciexyzY;
    int ciexyzZ;
} CIEXYZ;

typedef struct {
    CIEXYZ  ciexyzRed;
    CIEXYZ  ciexyzGreen;
    CIEXYZ  ciexyzBlue;
} CIEXYZTRIPLE;

#pragma pack(2)
typedef struct {
    unsigned short bfType;
    unsigned int   bfSize;
    unsigned short bfReserved1;
    unsigned short bfReserved2;
    unsigned int   bfOffBits;
} BITMAPFILEHEADER;

typedef struct {
    unsigned int   biSize;
    unsigned int   biWidth;
    unsigned int   biHeight;
    unsigned short biPlanes;
    unsigned short biBitCount;
    unsigned int   biCompression;
    unsigned int   biSizeImage;
    unsigned int   biXPelsPerMeter;
    unsigned int   biYPelsPerMeter;
    unsigned int   biClrUsed;
    unsigned int   biClrImportant;

} BITMAPINFOHEADER;
typedef struct {
    unsigned int   biRedMask;
    unsigned int   biGreenMask;
    unsigned int   biBlueMask;
    unsigned int   biAlphaMask;
    unsigned int   biCSType;
    CIEXYZTRIPLE   biEndpoints;
    unsigned int   biGammaRed;
    unsigned int   biGammaGreen;
    unsigned int   biGammaBlue;
} BITMAPV4INFOHEADER;

typedef struct {
    unsigned int   biIntent;
    unsigned int   biProfileData;
    unsigned int   biProfileSize;
    unsigned int   biReserved;
} BITMAPV5INFOHEADER;

class Bitmap {

private:
    int err;
    BITMAPFILEHEADER fileHeader;
    BITMAPINFOHEADER fileInfoHeader;
    BITMAPV4INFOHEADER filev4InfoHeader;
    BITMAPV5INFOHEADER filev5InfoHeader;
    RGBQUAD** imageData;

public:

    Bitmap();
    ~Bitmap();
    bool IsValidType(unsigned short);
    bool IsValidFormat(unsigned int bisize);
    bool IsValidBitcount(unsigned short bibitcount);
    bool IsValidCompression(unsigned int bicompression);
    bool IsValidProportions(unsigned int width, unsigned int height);
    bool IsEqualPixels(RGBQUAD a, RGBQUAD b);

    int ErrType();
    unsigned int Width();
    unsigned int Height();
    unsigned int BitCount();
    unsigned int FileSize();

    void SetErr(int val);
    void SetPixel(Point p, RGBQUAD color);
    Point RotatedPixel(Point p, Point p0, double angle);
    bool GetPixel(Point p, RGBQUAD* color);
    void DrawLine(Point p1, Point p2, unsigned int thick, RGBQUAD color);
    void DrawBresenhamLine(Point p1, Point p2, RGBQUAD color);
    void DrawFilledRectangle(Point p, unsigned int size, RGBQUAD color);
    void RegionFill(Point p, RGBQUAD f_col, RGBQUAD b_col);
    void PolygonFill(Point p1, Point p2, Point p3, RGBQUAD color);
    void PolygonRotatedFill(Point p1, Point p2, Point p3, Point rot, int scale);
    void PolygonRotatedFill(Point p1, Point p2, Point p3, Point rot, int scale, RGBQUAD** pixelData);
    void PolygonCopy(Point p1, Point p2, Point p3, Point rot, Bitmap* bmp);
    void Eraser (Point p, unsigned int size);
    void DrawCircle(Point p,unsigned int radius, unsigned int thick, RGBQUAD color);
    void DrawNpointedStar(Point p, unsigned int radius, unsigned int n, unsigned int thick, RGBQUAD color);
    void HorzMirror(Point p1, Point p2);
    void VertMirror(Point p1, Point p2);

    void Open(const char* filename);
    void SaveAs(const char *filename);
    void CreateNew(unsigned int width, unsigned int height, unsigned int bitcount, unsigned int version);
};

#endif // BITMAP_H
