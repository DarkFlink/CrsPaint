#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QFileDialog>
#include <QGraphicsScene>
#include <QGraphicsSceneMouseEvent>
#include <QColorDialog>
#include <QInputDialog>
#include <QMainWindow>
#include <QIcon>
#include <QMenu>
#include <QPixmap>
#include <QMessageBox>

#include "bitmap.h"
#include "newfiledialog.h"
#include "linethicknessdialog.h"
#include "courseworkaboutwidget.h"
#include "paintgraphicsscene.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void createMenuFile();
    void createMenuOptions();
    void createMenuTools();
    void createMenuAbout();
    void createGraphicsScene(bool savePaintConfigs, QString actionType);
    void updatePixmap(QString mFilename);
    void msgboxOutput(QString mMessage, QString mTitle, QString msgType);
    void setDefaultImageConfigs();
    QString errCatalog(int type);

private slots:

    void on_actionFile_triggered();
        void on_actionNew_triggered();
        void on_actionExit_triggered();
        void on_actionOpen_triggered();
        void on_actionSave_triggered();
        void on_actionSaveAs_triggered();

    void on_actionOptions_triggered();
        void on_actionLine_thickness_triggered();
        void on_actionPalette_triggered();
        void on_actionReset_parameters_triggered();
        void on_actionParameters_triggered();

    void on_actionTools_triggered();
        void on_actionDrawLine_triggered();
        void on_actionDrawCircle_triggered();
        void on_actionDrawStar_triggered();
        void on_actionVertMirror_triggered();
        void on_actionHorzMirror_triggered();
        void on_actionRegionFill_triggered();
        void on_actionEraser_triggered();

    void on_actionAbout_triggered();
        void on_actionFileinfo_triggered();
        void on_actionCourse_Paint_triggered();

        void on_actionDrawTriangle_triggered();

private:
    Ui::MainWindow *ui;

    CourseWorkAboutWidget* mCourseWorkAboutWidget;
    PaintGraphicsScene* mScene;
    PaintConfigs mPaintConfigs;
    ImageConfigs mImageConfigs;

    QMenu* mMenuFile;
    QMenu* mMenuOptions;
    QMenu* mMenuTools;
    QMenu* mMenuAbout;

    QString* qfilename;
};

#endif // MAINWINDOW_H
