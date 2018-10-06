#include "newfiledialog.h"
#include "ui_newfiledialog.h"

NewFileDialog::NewFileDialog(QDialog *parent) :
    QDialog(parent),
    ui(new Ui::NewFileDialog)
{
    ui->setupUi(this);
    NewFileDialog::setWindowTitle("Create new");
    NewFileDialog::setModal(true);

    mImageConfigs.bitcount = 24;
    mImageConfigs.width = 640;
    mImageConfigs.height = 480;
    mImageConfigs.version = 5;

    createEnabled = false;

    ui->radioButton24Bit->toggle();
    ui->spinBoxWidth->setMaximum(4450);
    ui->spinBoxHeight->setMaximum(4450);
    ui->spinBoxWidth->setValue(mImageConfigs.width);
    ui->spinBoxHeight->setValue(mImageConfigs.height);
}

NewFileDialog::~NewFileDialog()
{
    delete ui;
}

ImageConfigs NewFileDialog::bitmapConfigs()
{
    return mImageConfigs;
}

void NewFileDialog::on_pushButtonCancel_clicked()
{
    NewFileDialog::close();
}

void NewFileDialog::on_pushButtonOK_clicked()
{
    mImageConfigs.width = ui->spinBoxWidth->value();
    mImageConfigs.height = ui->spinBoxHeight->value();

    createEnabled = true;
    NewFileDialog::close();
}


void NewFileDialog::on_radioButton24Bit_toggled(bool checked)
{
    if(!checked)
        mImageConfigs.bitcount = 24;
}

void NewFileDialog::on_radioButton32Bit_toggled(bool checked)
{
    if(!checked)
        mImageConfigs.bitcount = 32;
}


bool NewFileDialog::CreateBMPEnabled() {
    return createEnabled;
}
