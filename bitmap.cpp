#include "bitmap.h"

template <typename Type>
void read(std::ifstream &fp, Type &result, std::size_t size) {
    fp.read(reinterpret_cast<char*>(&result), size);
}

template <typename Type>
void write(std::ofstream &fp, Type &result, std::size_t size) {
    fp.write(reinterpret_cast<char*>(&result), size);
}

template <typename Type>
void Swap(Type* a, Type* b) {
    Type c = *a;
    *a = *b;
    *b = c;
}

Bitmap::Bitmap() {}

Bitmap::~Bitmap() {
    for(unsigned int i=0;i<Height();i++)
        delete imageData[i];
}

bool Bitmap::IsValidType(unsigned short bftype) {
    if(bftype == 0x4D42)
        return true;
    else {
        return false;
    }
}

bool Bitmap::IsValidFormat(unsigned int bisize) {
    if (bisize == 40 || bisize == 52 || bisize == 56 || bisize == 108 || bisize == 124)
        return true;
    else {
        return false;
    }
}

bool Bitmap::IsValidBitcount(unsigned short bibitcount) {
    if(bibitcount == 24 || bibitcount == 32)
        return true;
    else {
        return false;
    }
}

bool Bitmap::IsValidCompression(unsigned int bicompression) {
    if(bicompression == 0 || bicompression == 3)
        return true;
    else {
        return false;
    }
}

bool Bitmap::IsValidProportions(unsigned int width, unsigned int height) {
    if(width && height)
        return true;
    else {
        return false;
    }
}

bool Bitmap::IsEqualPixels(RGBQUAD a, RGBQUAD b) {
    if(a.rgbBlue == b.rgbBlue && a.rgbGreen == b.rgbGreen && a.rgbRed == b.rgbRed)
        return true;
    else
        return false;
}

int Bitmap::ErrType() {
    return err;
}

void Bitmap::SetErr(int val) {
    err = val;
}

unsigned int Bitmap::Width() {
    return fileInfoHeader.biWidth;
}

unsigned int Bitmap::Height() {
    return fileInfoHeader.biHeight;
}

unsigned int Bitmap::BitCount() {
    return fileInfoHeader.biBitCount;
}

unsigned int Bitmap::FileSize() {
    return fileHeader.bfSize;
}

void Bitmap::SetPixel(Point p, RGBQUAD color) {
    if(p.x >= Width() || p.y >= Height())
        return;
    imageData[p.y][p.x] = color;
}

Point Bitmap::RotatedPixel(Point p, Point p0, double angle) {
    Point curr;
    double xc = round((double)p0.x + ((double)p.x - (double)p0.x)*cos(angle) - ((double)p.y - (double)p0.y)*sin(angle));
    double yc = round((double)p0.y + ((double)p.y - (double)p0.y)*cos(angle) + ((double)p.x - (double)p0.x)*sin(angle));
    curr.x = (int)xc;
    curr.y = (int)yc;
    return curr;
}

bool Bitmap::GetPixel(Point p, RGBQUAD* color) {
    if(p.x >= Width() || p.y >= Height())
        return false;
    else {
        *color = imageData[p.y][p.x];
        return true;
    }
}

void Bitmap::RegionFill(Point p, RGBQUAD f_col, RGBQUAD b_col){

    Swap(&p.x, &p.y);
    RGBQUAD color;
    if(!GetPixel(p, &color))
        return;

    if(!IsEqualPixels(color, f_col) && !IsEqualPixels(color, b_col)){

        SetPixel(p, f_col);
        RegionFill({p.x+1, p.y}, f_col, b_col);
        RegionFill({p.x-1, p.y}, f_col, b_col);
        RegionFill({p.x, p.y+1}, f_col, b_col);
        RegionFill({p.x, p.y-1}, f_col, b_col);
    }
}

void Bitmap::PolygonRotatedFill(Point p1, Point p2, Point p3, Point rot, int scale) {

    p1 = RotatedPixel(p1, rot, scale*ANGLE_72GRAD);
    p2 = RotatedPixel(p2, rot, scale*ANGLE_72GRAD);
    p3 = RotatedPixel(p3, rot, scale*ANGLE_72GRAD);

    if(p1.y > p2.y) {
        Swap(&p1.y ,&p2.y);
        Swap(&p1.x, &p2.x);
    }
    if(p2.y > p3.y) {
        Swap(&p3.y ,&p2.y);
        Swap(&p3.x, &p2.x);
    }
    if(p1.y > p2.y) {
        Swap(&p1.y ,&p2.y);
        Swap(&p1.x, &p2.x);
    }

    int xf, xs;
    for(int y = p1.y; y <= (int)p3.y; y++) {

        if(!((int)p3.y-(int)p1.y)&&!((int)p2.y-(int)p1.y))
            continue;

        xf = p1.x + (int)(y - p1.y) * (int)(p3.x - p1.x) / (int)(p3.y - p1.y);

        if(y < (int)p2.y){
            xs = p1.x + (int)(y-p1.y)*(int)(p2.x-p1.x)/(int)(p2.y-p1.y);
        }

        else {
            if(p3.y == p2.y)
                xs = p2.x;
            else
                xs = p2.x + ((int)y - (int)p2.y) * (int)(p3.x - p2.x) / (int)(p3.y - p2.y);
        }
        if(xf > xs) // xs > xf
            Swap(&xf,&xs);

        for(int i=xf; i<=xs; i++) {
            RGBQUAD color;
            Point original = RotatedPixel({(unsigned int)i,(unsigned int)y}, {rot.x,rot.y}, scale*ANGLE_72GRAD);
            GetPixel(original, &color);
            SetPixel({(unsigned int)i,(unsigned int)y}, color);
//            RotatePixel({(unsigned int)i,(unsigned int)y}, {220,220}, ANGLE_72GRAD);
        }
    }
}

void Bitmap::PolygonRotatedFill(Point p1, Point p2, Point p3, Point rot, int scale, RGBQUAD** pixelData) {

    p1 = RotatedPixel(p1, rot, scale*ANGLE_72GRAD);
    p2 = RotatedPixel(p2, rot, scale*ANGLE_72GRAD);
    p3 = RotatedPixel(p3, rot, scale*ANGLE_72GRAD);

    if(p1.y > p2.y) {
        Swap(&p1.y ,&p2.y);
        Swap(&p1.x, &p2.x);
    }
    if(p2.y > p3.y) {
        Swap(&p3.y ,&p2.y);
        Swap(&p3.x, &p2.x);
    }
    if(p1.y > p2.y) {
        Swap(&p1.y ,&p2.y);
        Swap(&p1.x, &p2.x);
    }

    int xf, xs;
    for(int y = p1.y; y <= (int)p3.y; y++) {

        if(!((int)p3.y-(int)p1.y)&&!((int)p2.y-(int)p1.y))
            continue;

        xf = p1.x + (int)(y - p1.y) * (int)(p3.x - p1.x) / (int)(p3.y - p1.y);

        if(y < (int)p2.y){
            xs = p1.x + (int)(y-p1.y)*(int)(p2.x-p1.x)/(int)(p2.y-p1.y);
        }

        else {
            if(p3.y == p2.y)
                xs = p2.x;
            else
                xs = p2.x + ((int)y - (int)p2.y) * (int)(p3.x - p2.x) / (int)(p3.y - p2.y);
        }
        if(xf > xs) // xs > xf
            Swap(&xf,&xs);

        for(int i=xf; i<=xs; i++) {
            RGBQUAD color;
            Point original = RotatedPixel({(unsigned int)i,(unsigned int)y}, {rot.x,rot.y}, scale*ANGLE_72GRAD);
            if(original.x >= Width() || original.y >= Height())
                continue;
            color = pixelData[original.y][original.x];
            pixelData[(unsigned int)i][(unsigned int)y] = color;
//            RotatePixel({(unsigned int)i,(unsigned int)y}, {220,220}, ANGLE_72GRAD);
        }
    }
}

void Bitmap::PolygonCopy(Point p1, Point p2, Point p3, Point rot, Bitmap* bmp)
{
    p1 = RotatedPixel(p1, rot, 1*ANGLE_72GRAD);
    p2 = RotatedPixel(p2, rot, 1*ANGLE_72GRAD);
    p3 = RotatedPixel(p3, rot, 1*ANGLE_72GRAD);

    if(p1.y > p2.y) {
        Swap(&p1.y ,&p2.y);
        Swap(&p1.x, &p2.x);
    }
    if(p2.y > p3.y) {
        Swap(&p3.y ,&p2.y);
        Swap(&p3.x, &p2.x);
    }
    if(p1.y > p2.y) {
        Swap(&p1.y ,&p2.y);
        Swap(&p1.x, &p2.x);
    }

    int xf, xs;
    for(int y = p1.y; y <= (int)p3.y; y++) {

        if(!((int)p3.y-(int)p1.y)&&!((int)p2.y-(int)p1.y))
            continue;

        xf = p1.x + (int)(y - p1.y) * (int)(p3.x - p1.x) / (int)(p3.y - p1.y);

        if(y < (int)p2.y){
            xs = p1.x + (int)(y-p1.y)*(int)(p2.x-p1.x)/(int)(p2.y-p1.y);
        }

        else {
            if(p3.y == p2.y)
                xs = p2.x;
            else
                xs = p2.x + ((int)y - (int)p2.y) * (int)(p3.x - p2.x) / (int)(p3.y - p2.y);
        }
        if(xf > xs) // xs > xf
            Swap(&xf,&xs);

        for(int i=xf; i<=xs; i++) {
            RGBQUAD color;
            bmp->GetPixel({(unsigned int)i,(unsigned int)y}, &color);
            SetPixel({(unsigned int)i,(unsigned int)y}, color);
        }
    }
}

void Bitmap::PolygonFill(Point p1, Point p2, Point p3, RGBQUAD color) {

    if(p1.y > p2.y) {
        Swap(&p1.y ,&p2.y);
        Swap(&p1.x, &p2.x);
    }
    if(p2.y > p3.y) {
        Swap(&p3.y ,&p2.y);
        Swap(&p3.x, &p2.x);
    }
    if(p1.y > p2.y) {
        Swap(&p1.y ,&p2.y);
        Swap(&p1.x, &p2.x);
    }

    int xf, xs;
    for(int y = p1.y; y <= (int)p3.y; y++) {

        if(!((int)p3.y-(int)p1.y)&&!((int)p2.y-(int)p1.y))
            continue;

        xf = p1.x + (int)(y - p1.y) * (int)(p3.x - p1.x) / (int)(p3.y - p1.y);

        if(y < (int)p2.y){
            xs = p1.x + (int)(y-p1.y)*(int)(p2.x-p1.x)/(int)(p2.y-p1.y);
        }

        else {
            if(p3.y == p2.y)
                xs = p2.x;
            else
                xs = p2.x + ((int)y - (int)p2.y) * (int)(p3.x - p2.x) / (int)(p3.y - p2.y);
        }
        if(xf > xs) // xs > xf
            Swap(&xf,&xs);

        DrawBresenhamLine({(unsigned int)xf,(unsigned int)y}, {(unsigned int)xs,(unsigned int)y},color);
    }
}

void Bitmap::DrawBresenhamLine(Point p1, Point p2, RGBQUAD color) {

    const int deltaX = abs((int)p2.x - (int)p1.x);
    const int deltaY = abs((int)p2.y - (int)p1.y);
    const int signX = (int)p1.x < (int)p2.x ? 1 : -1;
    const int signY = (int)p1.y < (int)p2.y ? 1 : -1;

    int error = deltaX - deltaY;

    SetPixel(p2, color);
    while(p1.x != p2.x || p1.y != p2.y)
    {
        SetPixel(p1, color);
        const int error2 = error * 2;

        if(error2 > -deltaY)
        {
            error -= deltaY;
            p1.x += signX;
        }
        if(error2 < deltaX)
        {
            error += deltaX;
            p1.y += signY;
        }
    }
}

void Bitmap::DrawLine(Point p1, Point p2, unsigned int thick, RGBQUAD color) {

    Swap(&p1.x, &p1.y);
    Swap(&p2.x, &p2.y);

    if(thick == 1) {
        DrawBresenhamLine(p1,p2,color);
        return;
    }

    double tg = double((int)(p2.y-p1.y))/(int)(p2.x-p1.x);
    double sin2 = 1/(1+1/(tg*tg));
    double cos2 = 1.0-sin2;

    int dy=thick*sqrt(cos2);
    int dx=thick*sqrt(sin2);

    if(tg > 0) {
        PolygonFill({p1.x+dx,p1.y-dy}, {p1.x-dx,p1.y+dy}, {p2.x+dx,p2.y-dy}, color);
        PolygonFill({p2.x+dx,p2.y-dy}, {p2.x-dx,p2.y+dy}, {p1.x-dx,p1.y+dy}, color);
    }
    else {
        PolygonFill({p1.x+dx,p1.y+dy}, {p1.x-dx,p1.y-dy}, {p2.x-dx,p2.y-dy}, color);
        PolygonFill({p1.x+dx,p1.y+dy}, {p2.x-dx,p2.y-dy}, {p2.x+dx,p2.y+dy}, color);
    }
}

void Bitmap::DrawFilledRectangle(Point p, unsigned int size, RGBQUAD color) {

    if(size==1) {
        Swap(&p.x, &p.y);
        SetPixel(p, color);
        return;
    }

    unsigned int x,y;
    x = p.x;
    y = p.y;

    for(unsigned int i=0;i < size;i++)
        DrawBresenhamLine({x+i,y},{x+i,y+size-1}, color);
}

void Bitmap::Eraser (Point p, unsigned int size) {

    RGBQUAD eraser(255,255,255);
    DrawFilledRectangle(p, size, eraser);
}

void Bitmap::DrawCircle(Point p, unsigned int radius, unsigned int thick, RGBQUAD color) {

    Swap(&p.x, &p.y);
    if(radius == 0) {
        SetPixel(p, color);
        return;
    }

    int x = 0;
    int y = radius;
    int delta = 1 - 2 * radius;
    int error = 0;
    while(y >= 0) {
        DrawFilledRectangle({p.x + x, p.y + y}, thick, color);
        DrawFilledRectangle({p.x + x, p.y - y}, thick, color);
        DrawFilledRectangle({p.x - x, p.y + y}, thick, color);
        DrawFilledRectangle({p.x - x, p.y - y}, thick, color);
        error = 2 * (delta + y) - 1;
        if(delta < 0 && error <= 0) {
            ++x;
            delta += 2 * x + 1;
            continue;
        }
        error = 2 * (delta - x) - 1;
        if(delta > 0 && error > 0) {
            --y;
            delta += 1 - 2 * y;
            continue;
        }
        ++x;
        delta += 2 * (x - y);
        --y;
    }
}

void Bitmap::DrawNpointedStar(Point p, unsigned int radius, unsigned int n, unsigned int thick, RGBQUAD color) {

    Swap(&p.x, &p.y);
    double a = 0, b = 3.14159265358979323846 / n,k=0.3;
    unsigned int x1=(p.x + (int)(radius * cos(a))), x2, y1=(p.y + (int)(radius * sin(a))), y2;

    Point insidePoints[5];
    Point outsidePoints[5];
    for (unsigned int i = 0; i < n; i++) {
        a += b;
        y2 = (p.y + (int)(k*radius * sin(a)));
        x2 = (p.x + (int)(k*radius * cos(a)));

        a += b;
        DrawFilledRectangle({x1, y1}, thick, color);
        DrawFilledRectangle({x2, y2}, thick, color);
        DrawLine({x1, y1}, {x2, y2}, thick, color);

        x1 = (p.x + (int)(radius * cos(a)));
        y1 = (p.y + (int)(radius * sin(a)));
        if(i<5) {
            insidePoints[i].x = y2;
            insidePoints[i].y = x2;
            outsidePoints[i].x = y1;
            outsidePoints[i].y = x1;
        }
        DrawLine({x1, y1}, {x2, y2}, thick, color);
    }
    DrawCircle({p.x, p.y}, radius, thick, color);
}

void Bitmap::HorzMirror(Point p1, Point p2) {

    if(p1.x > p2.x)
        Swap(&p1.x, &p2.x);
    if(p1.y > p2.y)
        Swap(&p1.y, &p2.y);

    int width = abs(p2.y - p1.y);
    int heigth = abs(p2.x - p1.x)/2;

    for(int j=0;j<heigth;j++) {
        for(int i=0; i<width; i++)
            Swap(&imageData[p1.x+j][p1.y+i], &imageData[p2.x-j][p1.y+i]);
    }
}

void Bitmap::VertMirror(Point p1, Point p2) {

    if(p1.x > p2.x)
        Swap(&p1.x, &p2.x);
    if(p1.y > p2.y)
        Swap(&p1.y, &p2.y);

    int width = abs(p2.y - p1.y)/2;
    int heigth = abs(p2.x - p1.x);

    for(int j=0;j<heigth;j++) {
        for(int i=0; i<width; i++)
            Swap(&imageData[p1.x+j][p1.y+i], &imageData[p1.x+j][p2.y-i-1]);
    }
}

void Bitmap::Open(const char* filename) {

    if(!filename)
        return;

    std::ifstream fileStream;
    fileStream.open(filename, std::ifstream::binary);
    if (!fileStream) {
        err = 1001;
        return;
    }

    // bmp header
    read(fileStream, fileHeader.bfType, sizeof(fileHeader.bfType));
    read(fileStream, fileHeader.bfSize, sizeof(fileHeader.bfSize));
    read(fileStream, fileHeader.bfReserved1, sizeof(fileHeader.bfReserved1));
    read(fileStream, fileHeader.bfReserved2, sizeof(fileHeader.bfReserved2));
    read(fileStream, fileHeader.bfOffBits, sizeof(fileHeader.bfOffBits));

    if(!IsValidType(fileHeader.bfType)) {
        err = 1002;
        return;
    }

    read(fileStream, fileInfoHeader.biSize, sizeof(fileInfoHeader.biSize));
    if(!IsValidFormat(fileInfoHeader.biSize)) {
        err = 1003;
        return;
    }

    // bmp info v3
    if (fileInfoHeader.biSize >= 40) {
        read(fileStream, fileInfoHeader.biWidth, sizeof(fileInfoHeader.biWidth));
        read(fileStream, fileInfoHeader.biHeight, sizeof(fileInfoHeader.biHeight));
        read(fileStream, fileInfoHeader.biPlanes, sizeof(fileInfoHeader.biPlanes));
        read(fileStream, fileInfoHeader.biBitCount, sizeof(fileInfoHeader.biBitCount));
        read(fileStream, fileInfoHeader.biCompression, sizeof(fileInfoHeader.biCompression));
        read(fileStream, fileInfoHeader.biSizeImage, sizeof(fileInfoHeader.biSizeImage));
        read(fileStream, fileInfoHeader.biXPelsPerMeter, sizeof(fileInfoHeader.biXPelsPerMeter));
        read(fileStream, fileInfoHeader.biYPelsPerMeter, sizeof(fileInfoHeader.biYPelsPerMeter));
        read(fileStream, fileInfoHeader.biClrUsed, sizeof(fileInfoHeader.biClrUsed));
        read(fileStream, fileInfoHeader.biClrImportant, sizeof(fileInfoHeader.biClrImportant));
    }

    if(!IsValidBitcount(fileInfoHeader.biBitCount)) {
        err = 1004;
        return;
    }

    if(!IsValidCompression(fileInfoHeader.biCompression)) {
        err = 1005;
        return;
    }

    if (fileInfoHeader.biSize >= 52) {
        read(fileStream, filev4InfoHeader.biRedMask, sizeof(filev4InfoHeader.biRedMask));
        read(fileStream, filev4InfoHeader.biGreenMask, sizeof(filev4InfoHeader.biGreenMask));
        read(fileStream, filev4InfoHeader.biBlueMask, sizeof(filev4InfoHeader.biBlueMask));
    }

    if (fileInfoHeader.biSize >= 56)
        read(fileStream, filev4InfoHeader.biAlphaMask, sizeof(filev4InfoHeader.biAlphaMask));

    if (fileInfoHeader.biSize >= 108) {
        read(fileStream, filev4InfoHeader.biCSType, sizeof(filev4InfoHeader.biCSType));
        read(fileStream, filev4InfoHeader.biEndpoints, sizeof(filev4InfoHeader.biEndpoints));
        read(fileStream, filev4InfoHeader.biGammaRed, sizeof(filev4InfoHeader.biGammaRed));
        read(fileStream, filev4InfoHeader.biGammaGreen, sizeof(filev4InfoHeader.biGammaGreen));
        read(fileStream, filev4InfoHeader.biGammaBlue, sizeof(filev4InfoHeader.biGammaBlue));
    }

    // bmp info v5
    if (fileInfoHeader.biSize >= 124) {
        read(fileStream, filev5InfoHeader.biIntent, sizeof(filev5InfoHeader.biIntent));
        read(fileStream, filev5InfoHeader.biProfileData, sizeof(filev5InfoHeader.biProfileData));
        read(fileStream, filev5InfoHeader.biProfileSize, sizeof(filev5InfoHeader.biProfileSize));
        read(fileStream, filev5InfoHeader.biReserved, sizeof(filev5InfoHeader.biReserved));
    }

    imageData = new RGBQUAD*[Height()];
    for (unsigned int i = 0; i < Height(); i++)
        imageData[i] = new RGBQUAD[Width()];

    int linePadding = ((Width() * (fileInfoHeader.biBitCount / 8)) % 4) & 3;
    for (int i = Height()-1; i >= 0; i--) {
        for (unsigned int j = 0; j < Width(); j++) {

            read(fileStream, imageData[i][j].rgbBlue, sizeof(imageData[i][j].rgbBlue));
            read(fileStream, imageData[i][j].rgbGreen, sizeof(imageData[i][j].rgbGreen));
            read(fileStream, imageData[i][j].rgbRed, sizeof(imageData[i][j].rgbRed));
            if(fileInfoHeader.biBitCount == 32)
                read(fileStream, imageData[i][j].rgbReserved, sizeof(imageData[i][j].rgbReserved));
        }
        fileStream.seekg(linePadding, std::ios_base::cur);
    }
    fileStream.close();
    return ;
}

void Bitmap::SaveAs(const char *filename) {

    if(!imageData || !filename)
        return;

    std::ofstream fileStream;
    fileStream.open(filename, std::ofstream::binary);

    write(fileStream, fileHeader, sizeof(BITMAPFILEHEADER));
    write(fileStream, fileInfoHeader, sizeof(BITMAPINFOHEADER));

    if(fileInfoHeader.biSize >= 108)
        write(fileStream, filev4InfoHeader, sizeof(BITMAPV4INFOHEADER));

    if(fileInfoHeader.biSize == 124)
        write(fileStream, filev5InfoHeader, sizeof(BITMAPV5INFOHEADER));

    int nil=0;
    if(fileInfoHeader.biBitCount == 32 )
        write(fileStream, nil, sizeof(char));

    int linePadding = ((Width() * (fileInfoHeader.biBitCount / 8)) % 4) & 3;
    for (int i = Height()-1; i >= 0; i--) {
        for (unsigned int j = 0; j < Width(); j++) {
            write(fileStream, imageData[i][j].rgbBlue, sizeof(imageData[i][j].rgbRed));
            write(fileStream, imageData[i][j].rgbGreen, sizeof(imageData[i][j].rgbGreen));
            write(fileStream, imageData[i][j].rgbRed, sizeof(imageData[i][j].rgbBlue));

            if(fileInfoHeader.biBitCount == 32)
                write(fileStream, imageData[i][j].rgbReserved, sizeof(imageData[i][j].rgbReserved));
        }
        write(fileStream, nil, linePadding);
    }

    fileStream.close();
    return;
}

void Bitmap::CreateNew(unsigned int width, unsigned int height, unsigned int bitcount, unsigned int version) {

    RGBQUAD w(255, 255, 255);
    if(!IsValidBitcount(bitcount)) {
        err = 2004;
        return;
    }
    if(!IsValidProportions(width,height)) {
        err = 2006;
        return;
    }

    fileHeader.bfType = 0x4D42;
    fileHeader.bfReserved1 = 0;
    fileHeader.bfReserved2 = 0;

    if(version != 3)
        fileInfoHeader.biSize = 124;
    else
        fileInfoHeader.biSize = 40;

    fileInfoHeader.biWidth = width;
    fileInfoHeader.biHeight = height;
    fileInfoHeader.biPlanes = 1;
    fileInfoHeader.biBitCount = bitcount;

    if(bitcount==24)
        fileInfoHeader.biCompression = 0;
    if(bitcount==32) {
        fileInfoHeader.biCompression = 3;
    }

    fileInfoHeader.biSizeImage = width*height*sizeof(RGBQUAD);
    fileInfoHeader.biXPelsPerMeter = 2835;
    fileInfoHeader.biYPelsPerMeter = 2835;
    fileInfoHeader.biClrUsed = 0;
    fileInfoHeader.biClrImportant = 0;

    if(version != 3) {
        filev4InfoHeader.biRedMask = 0xFF000000;
        filev4InfoHeader.biGreenMask = 0x00FF0000;
        filev4InfoHeader.biBlueMask = 0x0000FF00;
        filev4InfoHeader.biAlphaMask = 0x0;
    }

    imageData = new RGBQUAD*[fileInfoHeader.biHeight];
    for (unsigned int i = 0; i < fileInfoHeader.biHeight; i++) {
        imageData[i] = new RGBQUAD[fileInfoHeader.biWidth];
    }

    for (unsigned int i = 0; i < fileInfoHeader.biHeight; i++) {
        for (unsigned int j = 0; j < fileInfoHeader.biWidth; j++) {
            imageData[i][j] = w;
        }
    }

    fileHeader.bfSize = sizeof(BITMAPFILEHEADER) + fileInfoHeader.biSize + fileInfoHeader.biSizeImage;
    fileHeader.bfOffBits = sizeof(BITMAPFILEHEADER) + fileInfoHeader.biSize;
}
