#ifndef LINETHICKNESS_H
#define LINETHICKNESS_H

#include <QWidget>

namespace Ui {
class LineThicknessDialog;
}

class LineThicknessDialog : public QWidget
{
    Q_OBJECT

public:
    explicit LineThicknessDialog(QWidget *parent = 0);
    ~LineThicknessDialog();

private:
    Ui::LineThicknessDialog *ui;
    
    
};

#endif // LINETHICKNESS_H
