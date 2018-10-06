#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    qfilename = NULL;
    setDefaultImageConfigs();
    createGraphicsScene(false, "CreateNew");

    createMenuFile();
    createMenuOptions();
    createMenuTools();
    createMenuAbout();
    mCourseWorkAboutWidget = new CourseWorkAboutWidget;
}

MainWindow::~MainWindow()
{
    delete mScene;
    delete mCourseWorkAboutWidget;
    delete mMenuAbout;
    delete mMenuFile;
    delete mMenuOptions;
    delete mMenuTools;
    delete ui;
}

//---------------------//
//  Construct methods  //
//---------------------//

void MainWindow::createMenuFile() {

    mMenuFile = new QMenu;
    mMenuFile->setPalette(MainWindow::palette());

    mMenuFile->addAction(ui->actionNew);
    mMenuFile->addAction(ui->actionOpen);
    mMenuFile->addAction(ui->actionSave);
    mMenuFile->addAction(ui->actionSaveAs);
    mMenuFile->addSeparator();
    mMenuFile->addAction(ui->actionExit);
}

void MainWindow::createMenuOptions() {

    mMenuOptions = new QMenu;
    mMenuOptions->setPalette(MainWindow::palette());

    mMenuOptions->addAction(ui->actionLine_thickness);
    mMenuOptions->addAction(ui->actionPalette);
    mMenuOptions->addSeparator();
    mMenuOptions->addAction(ui->actionParameters);
    mMenuOptions->addAction(ui->actionReset_parameters);
}

void MainWindow::createMenuTools() {

    mMenuTools = new QMenu;
    mMenuTools->setPalette(MainWindow::palette());

    mMenuTools->addAction(ui->actionDrawLine);
    mMenuTools->addAction(ui->actionDrawCircle);
    mMenuTools->addAction(ui->actionDrawStar);
    mMenuTools->addAction(ui->actionVertMirror);
    mMenuTools->addAction(ui->actionHorzMirror);
    mMenuTools->addAction(ui->actionRegionFill);
    mMenuTools->addAction(ui->actionEraser);
}


void MainWindow::createMenuAbout() {

    mMenuAbout = new QMenu;
    mMenuAbout->setPalette(MainWindow::palette());

    mMenuAbout->addAction(ui->actionFileinfo);
    mMenuAbout->addSeparator();
    mMenuAbout->addAction(ui->actionCourse_Paint);
}

void MainWindow::createGraphicsScene(bool savePaintConfigs, QString actionType)
{
    mScene = new PaintGraphicsScene;
    if(!savePaintConfigs)
        mScene->setDefaultPaintConfigs();
    else
        mScene->setPaintConfigs(mPaintConfigs);

    mScene->bmp = new Bitmap;
    mScene->bmp->SetErr(0);

    if(actionType == "CreateNew") {
        mScene->bmp->CreateNew(mImageConfigs.width, mImageConfigs.height, mImageConfigs.bitcount, mImageConfigs.version);
    }
    if(actionType == "Open") {
        QByteArray ba = (*qfilename).toLatin1();
        const char* cfileName = ba.data();
        mScene->bmp->Open(cfileName);
    }
    msgboxOutput(errCatalog(mScene->bmp->ErrType()), "Error", "Error");

    mScene->bmp->SaveAs("temp.bmp");
    mScene->setPixmap("temp.bmp");
    ui->mGraphicsView->setScene(mScene);
    ui->mGraphicsView->setMinimumWidth(mScene->bmp->Width()+2);
    ui->mGraphicsView->setMinimumHeight(mScene->bmp->Height()+2);
}

//-------------------------//
//  Inteface action slots  //
//-------------------------//

void MainWindow::on_actionExit_triggered()
{
    QMessageBox msgBox;
    msgBox.setPalette(MainWindow::palette());
    QPoint mPoint;
    mPoint.setX(MainWindow::width()/3);
    mPoint.setY(MainWindow::height()/3);
    msgBox.move(MainWindow::mapToGlobal(mPoint));
    msgBox.setWindowTitle("Exit");
    msgBox.setText("Are you sure want\n          to exit?");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);

    int res = msgBox.exec();
    if(res == QMessageBox::Ok)
        MainWindow::close();
    else
        msgBox.close();
}

void MainWindow::on_actionNew_triggered() {

    NewFileDialog* mDialog = new NewFileDialog;

    QPoint mPoint;
    mPoint.setX(MainWindow::width()/3);
    mPoint.setY(MainWindow::height()/3);
    mDialog->move(MainWindow::mapToGlobal(mPoint));
    mDialog->exec();

    if(mDialog->CreateBMPEnabled()) {
        delete mScene;
        mImageConfigs = mDialog->bitmapConfigs();
        createGraphicsScene(false, "CreateNew");
    }
    delete mDialog;
}

void MainWindow::on_actionDrawLine_triggered()
{
    mScene->setDrawMode("Line");
}

void MainWindow::on_actionFileinfo_triggered()
{
    QString mString, bit, wi, hi, fs;
    bit = QString::number(mScene->bmp->BitCount());
    wi = QString::number(mScene->bmp->Width());
    hi = QString::number(mScene->bmp->Height());
    fs = QString::number(mScene->bmp->FileSize());
    mString = QString("BitCount:      %1\nWidth:      %2 px\nHeight:      %3 px\nFile size:      %4 bytes").arg(bit).arg(wi).arg(hi).arg(fs);

    msgboxOutput(mString, "About bmp", "Info");
}

void MainWindow::on_actionFile_triggered()
{
    QPoint mo;
    mo.setX(ui->toolBarMenu->pos().x()+4);
    mo.setY(ui->toolBarMenu->pos().y()+42);
    mMenuFile->move(ui->toolBarMenu->mapToGlobal(mo));

    if(!ui->actionFile->isCheckable())
        ui->actionFile->setCheckable(true);

    ui->actionFile->setChecked(true);
    mMenuFile->exec();
    ui->actionFile->setChecked(false);
}

void MainWindow::on_actionOpen_triggered()
{
    qfilename = new QString;
    *qfilename = QFileDialog::getOpenFileName(
                this,
                tr("Open file"),
                "~/",
                "All files (*.*);;Bitmap file (*.bmp)");
    mPaintConfigs = mScene->paintConfigs();
    delete mScene;
    createGraphicsScene(true, "Open");
}

void MainWindow::on_actionSave_triggered()
{
    if(!qfilename) {
        on_actionSaveAs_triggered();
        return;
    }

    const char* filename = (*qfilename).toUtf8().data();
    mScene->bmp->SaveAs(filename);
}

void MainWindow::on_actionSaveAs_triggered()
{
    qfilename = new QString;
    *qfilename = QFileDialog::getSaveFileName(
                this,
                tr("Save as file"),
                "~/",
                "All files (*.*);;Bitmap file (*.bmp)");

    const char* filename = (*qfilename).toUtf8().data();
    mScene->bmp->SaveAs(filename);
}

void MainWindow::on_actionOptions_triggered()
{
    QPoint mo;
    mo.setX(ui->toolBarMenu->pos().x()+52);
    mo.setY(ui->toolBarMenu->pos().y()+34);
    mMenuOptions->move(ui->toolBarMenu->mapToGlobal(mo));

    if(!ui->actionOptions->isCheckable())
        ui->actionOptions->setCheckable(true);

    ui->actionOptions->setChecked(true);
    mMenuOptions->exec();
    ui->actionOptions->setChecked(false);
}

void MainWindow::on_actionAbout_triggered()
{
    QPoint mo;
    mo.setX(ui->toolBarMenu->pos().x()+188);
    mo.setY(ui->toolBarMenu->pos().y()+34);
    mMenuAbout->move(ui->toolBarMenu->mapToGlobal(mo));

    if(!ui->actionAbout->isCheckable())
        ui->actionAbout->setCheckable(true);

    ui->actionAbout->setChecked(true);
    mMenuAbout->exec();
    ui->actionAbout->setChecked(false);
}

void MainWindow::on_actionTools_triggered()
{
    QPoint mo;
    mo.setX(ui->toolBarMenu->pos().x()+129);
    mo.setY(ui->toolBarMenu->pos().y()+34);
    mMenuTools->move(ui->toolBarMenu->mapToGlobal(mo));

    if(!ui->actionTools->isCheckable())
        ui->actionTools->setCheckable(true);

    ui->actionTools->setChecked(true);
    mMenuTools->exec();
    ui->actionTools->setChecked(false);
}

void MainWindow::on_actionLine_thickness_triggered()
{
    QInputDialog* mDialog = new QInputDialog;
    mScene->setPaintLineThick((unsigned int)mDialog->getInt(this, "Line thickness", "Enter thickness:", mScene->paintConfigs().thickLine));
    delete mDialog;
}

void MainWindow::on_actionPalette_triggered()
{
    QColor mColor;
    QColorDialog* mDialog = new QColorDialog;

    mColor = mDialog->getColor(Qt::black, this, "Color palette");
    if(mColor.blue()>255 || mColor.green()>255 || mColor.red()>255)
        msgboxOutput("Invalid color (too big color depth)", "Error", "Error");

    mScene->setPaintColor({(unsigned char)mColor.blue(), (unsigned char)mColor.green(), (unsigned char)mColor.red()});
}

void MainWindow::on_actionParameters_triggered()
{
    QString mString, lt, cr, svn, fsr, fsg, fsb;
    lt = QString::number(mScene->paintConfigs().thickLine);
    svn = QString::number(mScene->paintConfigs().starVertexNum);
    fsr = QString::number(mScene->paintConfigs().color.rgbRed);
    fsg = QString::number(mScene->paintConfigs().color.rgbGreen);
    fsb = QString::number(mScene->paintConfigs().color.rgbBlue);
    mString = QString("Line thickness:      %1\nStar vertex N:      %2\nColor:      %3 %4 %5").arg(lt).arg(svn).arg(fsr).arg(fsg).arg(fsb);

    msgboxOutput(mString, "Paint config", "Info");
}

void MainWindow::on_actionCourse_Paint_triggered()
{
    QPoint mPoint;
    mPoint.setX(MainWindow::width()/3);
    mPoint.setY(MainWindow::height()/3);

    mCourseWorkAboutWidget->move(MainWindow::mapToGlobal(mPoint));
    mCourseWorkAboutWidget->show();
}

void MainWindow::on_actionReset_parameters_triggered()
{
    mScene->setDefaultPaintConfigs();
}

void MainWindow::on_actionDrawCircle_triggered()
{
    mScene->setDrawMode("Circle");
}

void MainWindow::on_actionDrawStar_triggered()
{
    mScene->setDrawMode("NPointedStar");
}

void MainWindow::on_actionVertMirror_triggered()
{
    mScene->setDrawMode("VertMirror");
}

void MainWindow::on_actionHorzMirror_triggered()
{
    mScene->setDrawMode("HorzMirror");
}

void MainWindow::on_actionRegionFill_triggered()
{
    mScene->setDrawMode("RegionFill");
}

void MainWindow::on_actionDrawTriangle_triggered() {
    mScene->bmp->PolygonFill({200,200}, {250,100}, {100,170}, {50,50,50});
    mScene->bmp->DrawCircle({200,200}, 30, 6, {90, 111, 244});
    mScene->bmp->PolygonRotatedFill({200,200}, {250,100}, {100,170}, {220,220}, 2);
}

void MainWindow::on_actionEraser_triggered()
{
    mScene->setDrawMode("Eraser");
}

//-----------------//
//  Other methods  //
//-----------------//

void MainWindow::msgboxOutput(QString mMessage, QString mTitle, QString msgType) {

    if(msgType == "Error" && mScene->bmp->ErrType() == 0)
        return;

    QMessageBox* msgBox = new QMessageBox;
    msgBox->setWindowTitle(mTitle);
    msgBox->setText(mMessage);

    if(msgType == "Error")
        msgBox->setIcon(QMessageBox::Critical);

    QPoint mPoint;
    mPoint.setX(MainWindow::width()/3);
    mPoint.setY(MainWindow::height()/3);
    msgBox->move(MainWindow::mapToGlobal(mPoint));

    msgBox->exec();
    delete msgBox;
}

void MainWindow::setDefaultImageConfigs()
{
    mImageConfigs.bitcount = 24;
    mImageConfigs.height = 480;
    mImageConfigs.width = 640;
    mImageConfigs.version = 5;
}

QString MainWindow::errCatalog(int type) {
    switch(type) {
        case 1001:
            return "Opening file error (Invalid file)";

        case 1002:
            return "Opening file error (Invalid file format, not BMP)";

        case 1003:
            return "Opening file error (Invalid file header type)";

        case 1004:
            return "Opening file error (Invalid BMP bit count)";

        case 1005:
            return "Opening file error (Invalid BMP compression)";

        case 2004:
            return "Creating new file error (Invalid BMP bit count)";

        case 2006:
            return "Creating new file error (Invalid image proportions)";

        default:
            return "\0";
    }
}
