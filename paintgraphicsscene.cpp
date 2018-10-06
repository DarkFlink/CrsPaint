#include "paintgraphicsscene.h"

PaintGraphicsScene::PaintGraphicsScene(QObject *parent)
    : QGraphicsScene(parent)
{
    mMouseEnabled = false;
    mDrawMode = "NoMode";
    mPixmap = new QPixmap("temp.bmp");
}

PaintGraphicsScene::~PaintGraphicsScene()
{
    delete mPixmap;
    delete bmp;
}

void PaintGraphicsScene::mousePressEvent(QGraphicsSceneMouseEvent* mouseEvent)
{
    if(mouseEvent->button() == Qt::LeftButton) {
        mBegin = mouseEvent->scenePos();
        mMouseEnabled = true;
    }
}

void PaintGraphicsScene::mouseMoveEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if(mMouseEnabled && mDrawMode == "Eraser") {
        mPoint = mouseEvent->scenePos();
        paintEvent();
    }
}

void PaintGraphicsScene::mouseReleaseEvent(QGraphicsSceneMouseEvent *mouseEvent)
{
    if(mouseEvent->button() == Qt::LeftButton && mMouseEnabled) {
        if(mDrawMode != "RegionFill" && mDrawMode != "Eraser") {
            mEnd = mouseEvent->scenePos();
        }

        mMouseEnabled = false;
        paintEvent();
        bmp->SaveAs("temp.bmp");
        setPixmap("temp.bmp");

        if(mDrawMode != "Eraser")
            mDrawMode = "NoMode";
    }
}

void PaintGraphicsScene::paintEvent() {

    if(mDrawMode == "NoMode")
        return;

    if(mDrawMode == "Line") {
        Point p1(mBegin.toPoint().x(), mBegin.toPoint().y());
        Point p2(mEnd.toPoint().x(), mEnd.toPoint().y());
        bmp->DrawLine(p1, p2, mOptions.thickLine, mOptions.color);
    }

    if(mDrawMode == "Circle") {
        Point p1(mBegin.toPoint().x(), mBegin.toPoint().y());
        Point p2(mEnd.toPoint().x(), mEnd.toPoint().y());

        double dx = abs((int)p2.x - (int)p1.x);
        double dy = abs((int)p2.y - (int)p1.y);
        double rad = sqrt((dx*dx)+(dy*dy));
        bmp->DrawCircle(p1, (unsigned int)rad, mOptions.thickLine, mOptions.color);
    }

    if(mDrawMode == "NPointedStar") {
        Point p1(mBegin.toPoint().x(), mBegin.toPoint().y());
        Point p2(mEnd.toPoint().x(), mEnd.toPoint().y());

        double dx = abs((int)p2.x - (int)p1.x);
        double dy = abs((int)p2.y - (int)p1.y);
        double rad = sqrt((dx*dx)+(dy*dy));
        bmp->DrawNpointedStar(p1, (unsigned int)rad, mOptions.starVertexNum, mOptions.thickLine, mOptions.color);
    }

    if(mDrawMode == "VertMirror") {
        Point p1(mBegin.toPoint().x(), mBegin.toPoint().y());
        Point p2(mEnd.toPoint().x(), mEnd.toPoint().y());
        bmp->VertMirror(p1, p2);
    }

    if(mDrawMode == "HorzMirror") {
        Point p1(mBegin.toPoint().x(), mBegin.toPoint().y());
        Point p2(mEnd.toPoint().x(), mEnd.toPoint().y());
        bmp->HorzMirror(p1, p2);
    }

    if(mDrawMode == "RegionFill") {
        Point p(mBegin.toPoint().x(), mBegin.toPoint().y());
        bmp->RegionFill(p, mOptions.color, mOptions.color);
    }

    if(mDrawMode == "Eraser") {
        Point p(mPoint.toPoint().x(), mPoint.toPoint().y());
        bmp->Eraser(p, mOptions.thickLine);
    }
}

void PaintGraphicsScene::setDefaultPaintConfigs() {
    mOptions.color.rgbBlue = 0;
    mOptions.color.rgbGreen = 0;
    mOptions.color.rgbRed = 0;
    mOptions.color.rgbReserved = 0;
    mOptions.starVertexNum = 5;
    mOptions.thickLine = 1;
}

void PaintGraphicsScene::setDrawMode(QString drawMode) {
    mDrawMode = drawMode;
}

void PaintGraphicsScene::setPaintConfigs(PaintConfigs options)
{
    mOptions = options;
}

void PaintGraphicsScene::setPaintColor(RGBQUAD color) {
    mOptions.color = color;
}

void PaintGraphicsScene::setPaintLineThick(int thickness) {
    mOptions.thickLine = thickness;
}

PaintConfigs PaintGraphicsScene::paintConfigs() {
    return mOptions;
}

void PaintGraphicsScene::setPixmap(QString mFilename) {
    delete mPixmap;
    mPixmap = new QPixmap(mFilename);
    PaintGraphicsScene::addPixmap(*mPixmap);
}
