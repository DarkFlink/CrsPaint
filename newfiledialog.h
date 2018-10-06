#ifndef NEWFILEDIALOG_H
#define NEWFILEDIALOG_H

#include <QDialog>
#include "configs.h"

namespace Ui {
class NewFileDialog;
}

class NewFileDialog : public QDialog
{
    Q_OBJECT

public:
    explicit NewFileDialog(QDialog *parent = 0);
    ~NewFileDialog();

    ImageConfigs bitmapConfigs();

    bool CreateBMPEnabled();

private slots:

    void on_pushButtonCancel_clicked();
    void on_pushButtonOK_clicked();

    void on_radioButton24Bit_toggled(bool checked);
    void on_radioButton32Bit_toggled(bool checked);

private:
    Ui::NewFileDialog *ui;

    ImageConfigs mImageConfigs;

    bool createEnabled;
};

#endif // NEWFILEDIALOG_H
