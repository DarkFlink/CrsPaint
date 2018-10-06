#include "courseworkaboutwidget.h"
#include "ui_courseworkaboutwidget.h"

CourseWorkAboutWidget::CourseWorkAboutWidget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CourseWorkAboutWidget)
{
    ui->setupUi(this);
}

CourseWorkAboutWidget::~CourseWorkAboutWidget()
{
    delete ui;
}
