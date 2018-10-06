#include "linethicknessdialog.h"
#include "ui_linethickness.h"

LineThicknessDialog::LineThicknessDialog(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LineThicknessDialog)
{
    ui->setupUi(this);
}

LineThicknessDialog::~LineThicknessDialog()
{
    delete ui;
}
