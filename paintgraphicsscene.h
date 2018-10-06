#ifndef PAINTGRAPHICSSCENE_H
#define PAINTGRAPHICSSCENE_H

#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QMouseEvent>
#include <QMenu>
#include "bitmap.h"
#include "configs.h"

class PaintGraphicsScene : public QGraphicsScene
{
    Q_OBJECT

public:
    PaintGraphicsScene(QObject *parent = 0);
    ~PaintGraphicsScene();

    void mousePressEvent(QGraphicsSceneMouseEvent* mouseEvent) override;
    void mouseMoveEvent(QGraphicsSceneMouseEvent* mouseEvent) override;
    void mouseReleaseEvent(QGraphicsSceneMouseEvent* mouseEvent) override;

    void paintEvent();
    PaintConfigs paintConfigs();
    void setDrawMode(QString drawMode);
    void setPaintConfigs(PaintConfigs options);
    void setPaintColor(RGBQUAD color);
    void setPaintLineThick(int thickness);
    void setPixmap(QString mFilename);
    void setDefaultPaintConfigs();

    Bitmap* bmp;
private:

    bool mMouseEnabled;
    QPixmap* mPixmap;

    QPointF mBegin;
    QPointF mEnd;
    QPointF mPoint;
    QString mDrawMode;
    PaintConfigs mOptions;
};

#endif // PAINTGRAPHICSSCENE_H
